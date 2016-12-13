/**
 *  Filename: matrix_graph.c
 *   Created: 2016-12-08 12:49:00
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <malloc.h>
#include <limits.h>
#include <assert.h>
#define V 5
typedef struct __edata
{
    int start;
    int end;
    int weight;
}edata_t; //描述边的结构体
typedef struct __matrix_graph
{
    char vexs[V];
    int vex_num;
    int edge_num;
    int matrix[V][V];
}matrix_graph_t;
//不适合负权值
/* 创建以下的图
 *         2    3
 *      (0)--(1)--(2)
 *       |   / \   |
 *      6| 8/   \5 |7
 *       | /     \ |
 *      (3)-------(4)
 *          9          */

int dijkstra(int source, int graph[V][V])
{
    int i = 0;
    int j = 0;
    int k = 0;
    int dist[V];
    int parent[V];

    int flag[V];    //flag[i] == 1 表示“顶点s到顶点i的最短路径已经成功获取
    int tmp = 0;  //用来防止graph[k][i] + dist[k]溢出，如果graph[k][i] == INT_MAX, graph[k][i] + dist[k]会溢出
    int min = INT_MAX;

    for(i = 0; i < V; i++)
    {
        flag[i] = 0;
        dist[i] = graph[source][i];
        printf("source:%d---->vex:%d  dist:%d\n", source, i, graph[source][i]);
    }
    parent[source] = -1;
    flag[source] = 1;   //起始顶点先打入到集合中
    for(j = 0; j < V; j++)
    {
        min = INT_MAX;
        for(i = 0; i < V; i++)
        {
            if(flag[i] == 0)
            {
                if(dist[i] < min)
                {
                    min = dist[i];
                    k = i;  //与集合距离最短的顶点的下标
                }
            }
        }
        flag[k] = 1; //加入到集合中

        //更新距离
        for(i = 0; i < V; i++)
        {
            if(flag[i] == 0)
            {
                if(graph[k][i] < INT_MAX)
                {
                    if(dist[i] > dist[k] + graph[k][i])
                    {
                        printf("before dist:%d\n", dist[i]);
                        printf("k == %d\n", k);
                        dist[i] = dist[k] + graph[k][i];
                        printf("i == %d\n", i);
                        printf("dist:%d\n", dist[i]);
                        parent[i] = k;
                    }
                }
            }
        }
    }

    printf("dijkstra:\n");
    printf("source    vex    dist\n");
    for(i = 0; i < V; i++)
    {
        printf("%-6d", source);
        printf("%4s", " ");
        printf("%-3d", i);
        printf("%4s", " ");
        printf("%-4d", dist[i]);
        printf("\n");
    }
            
}
edata_t* get_edges(matrix_graph_t* mg)
{
    int i = 0;
    int j = 0;
    edata_t* edge = calloc(mg->edge_num, sizeof(edata_t));
    int k = 0; //用来指示
    int l = 0;
    int add = 0;
    for(i = 0; i < mg->vex_num; i++)
    {
        for(j = i + 1; j < mg->vex_num; j++)
        {
            if(mg->matrix[i][j] != INT_MAX)
            {
                add = 1;
                for(l = 0; l <= k; l++)
                {
                    //边去重
                    if(edge[l].start == j && edge[l].end == i)
                    {
                        add = 0;
                    }
                }
                if(add == 1)
                {
                    edge[k].start = i;
                    edge[k].end = j;
                    edge[k].weight = mg->matrix[i][j];
                    k++;
                }
            }
        }
    }
    printf("k == %d\n", k);
    assert(k == mg->edge_num);
    return edge; 
}
int sort_edges(edata_t* edge, int edge_num)
{
    int i = 0;
    int j = 0;
    edata_t tmp;

    int sorted_rank = 0;

    for(i = edge_num; i >= 2 ; i--)
    {
        for(j = 0; j < i - 1; j++)
        {
            if(edge[j].weight > edge[j + 1].weight)
            {
                tmp = edge[j + 1];
                edge[j + 1] = edge[j];
                edge[j] = tmp;
                sorted_rank = j;   //记录该序列中已经有序的起始位置
            }
        }
    }
    return 0;
}
int tree_union(int v1, int v2, int* root, int num)
{
    int root_v1 = root[v1];
    int root_v2 = root[v2];
    int i = 0;

    if(root_v1 != root_v2)
    {
        root[v2] = root_v1;
        //将顶点集中以root_v2为树根的顶点的树根改为以root_v1为树根
        for(i = 0; i < num; i++)
        {
            if(root[i] == root_v2)
            {
                root[i] = root_v1;
            }
        }
    }

}
int kruskal(matrix_graph_t* mg)
{
    int root[V];
    edata_t* edge = NULL;
    edge = get_edges(mg);
    sort_edges(edge, mg->edge_num);
    int i = 0;
    int m = 0;
    int n = 0;
    int v1;
    int v2;
    //开始时，V个顶点的图，每个顶点我们看成一个树, 开始时， 每个顶点的根都是其
    //本身
    for(i = 0; i < V; i++)
    {
        root[i] = i;
    }
    for(i = 0; i < mg->edge_num; i++)
    {
        v1 = edge[i].start;
        v2 = edge[i].end;
        //如果两个顶点属于不同的树
        if(root[v1] != root[v2])
        {
            printf("%d---->%d\n", v1, v2);
            tree_union(v1, v2, root, V);
        }
    }
}
int primMST(int graph[V][V])
{
    int mst_set[V];  //当前包含在MST中的点的集合
    int out[V];      //用来输出最小生成树
    int node_num = 0; //用来描述mst_set集合中节点的数目
    int weight[V];   //存储起始顶点到其余顶点的权重值
    int i = 0;
    int j = 0;
    int k = 0;      //顶点A 属于 mst_set集合，B属于非mst_set集合，A->B为当前权重最小的边，那么k就是顶点B在weight数组中的下标
    int min = INT_MAX;
    //0号节点加入到集合中
    mst_set[0] = 1;
    out[node_num] = 0;
    node_num++;
    for(i = 0;  i < V; i++)
    {
        weight[i] = graph[0][i];
    }
    while(node_num <= V)
    {
        min = INT_MAX;
        for(i = 1; i < V; i++)
        {
            if(mst_set[i] != 1)
            {
                if(min > weight[i])
                {
                    min = weight[i];
                    k = i;
                }
            }
        }
        //k指向的顶点将要加入到mst_set中
        mst_set[k] = 1;
        printf("k: %d\n", k);
        out[node_num] = k;
        node_num++;
        //加入新节点后，mst_set集合到非mst_set集合权重最小的边会发生变化
        for(j = 1; j < V; j++)
        {
            if(mst_set[j] != 1)
            {
                if(weight[j] > graph[k][j])
                {
                    weight[j] = graph[k][j];
                }
            }
        }
    }
    printf("MST:\n");
    for(i = 0; i < V; i++)
    {
        printf("%d", out[i]);
        printf("--->");
    }
    printf("\n");
    return 0;
}
/* 创建以下的图
 *         2    3
 *      (0)--(1)--(2)
 *       |   / \   |
 *      6| 8/   \5 |7
 *       | /     \ |
 *      (3)-------(4)
 *          9          */
int main(int argc, char* argv[])
{
    int graph[V][V] = {{0,       2, INT_MAX, 6,       INT_MAX},
                       {2,       0,    3,    8,             5},
                       {INT_MAX, 3,    0,    INT_MAX,       7},
                       {6,       8, INT_MAX, 0,             9},
                       {INT_MAX, 5,    7,    9,             0}
    };
    /* primMST(graph); */
    dijkstra(0, graph);
    matrix_graph_t mg;
    mg.vex_num = V;
    mg.edge_num = 7;
    memcpy(&mg.matrix, graph, sizeof(graph));
    /* kruskal(&mg); */
    return 0;
}
