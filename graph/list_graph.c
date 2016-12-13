/**
 *  Filename: list_graph.c
 *   Created: 2016-12-07 10:24:58
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//vnode2 状态
enum
{
    DISCOVERD,
    UNDISCOVERD,
    VISITED
};


enum
{
    BACK,    //后代指向祖先的边
    TREE     //属于正常的边
};

typedef struct __enode
{
    int ivex;
    struct __enode* next_edge;
}enode_t;

typedef struct __vnode
{
    char data;        //顶点数据
    enode_t* first_edge;    //指向第一条依附该顶点的弧
}vnode_t;

typedef struct __enode2
{
    int ivex;
    struct __enode2* next_edge;
    int property;
}enode2_t;

typedef struct __vnode2
{
    char data;
    enode2_t* first_edge;
}vnode2_t;


//邻接表
typedef struct __list_graph
{
    uint32_t vex_num;
    uint32_t edge_num;

    vnode_t* vex;
}list_graph_t;

//第二种邻接表, 为了支持判断图中是否有环，进而确定能否进行拓扑排序
typedef struct __list_graph2
{
    int vex_num;
    int edge_num;

    vnode2_t* vex;
}list_graph2_t;

typedef struct __queue
{
    int write;
    int read;
    int* array;
}queue_t;

typedef struct __stack
{
    int* array;
    int top;  //指向栈顶元素
}stack_t;
int get_vex_pos(list_graph_t* lg, char data)
{
    int i = 0;
    int index = -1;
    if(lg == NULL)
    {
        goto ERR;
    }
    else
    {
        for(i = 0; i < (int)lg->vex_num; i++)
        {
            if(lg->vex[i].data == data)
            {
                index = i;
                break;
            }

        }
    }
    if(index != -1)
    {
        return index;
    }
    else
    {
        return -1;
    }
ERR:
    return -1;

}
int get_vex_pos2(list_graph2_t* lg, char data)
{
    int i = 0;
    int index = -1;
    if(lg == NULL)
    {
        goto ERR;
    }
    else
    {
        for(i = 0; i < (int)lg->vex_num; i++)
        {
            if(lg->vex[i].data == data)
            {
                index = i;
                break;
            }

        }
    }
    if(index != -1)
    {
        return index;
    }
    else
    {
        return -1;
    }
ERR:
    return -1;
}


int create_example_list_graph(list_graph_t* lg)
{
    int i = 0;
    int start = 0;
    int end = 0;
    char c1 = 0;
    char c2 = 0;
    enode_t* enode = NULL; 

    char vex[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    char edges[][2] = { 
        {'A', 'G'}, 
        {'A', 'D'},
        {'B', 'A'},
        {'B', 'D'},
        {'C', 'F'},
        {'C', 'G'},
        {'D', 'E'},
        {'D', 'F'}
    };

    int vex_num = sizeof(vex) / sizeof(vex[0]);

    int edge_num = sizeof(edges) / sizeof(edges[0]);

    lg->vex_num = vex_num;
    lg->edge_num = edge_num;

    lg->vex = calloc(lg->vex_num, sizeof(vnode_t));

    if(lg->vex == NULL)
    {
        printf("calloc failed!\n");
        return -1;
    }

    for(i = 0; i < (int)lg->vex_num; i++)
    {
        lg->vex[i].data = vex[i];
        lg->vex[i].first_edge = NULL;
    }


    for(i = 0; i < (int)lg->edge_num; i++)
    {
        c1 = edges[i][0];
        c2 = edges[i][1];
        start = get_vex_pos(lg, c1);
        end = get_vex_pos(lg, c2);
        enode = (enode_t*)malloc(sizeof(enode_t));
        enode->ivex = end;
        enode->next_edge = lg->vex[start].first_edge;
        lg->vex[start].first_edge = enode;
    }
    return 0;

}
//出现环, 用来检测tsort能否规避环
int create_example_list_graph2(list_graph2_t* lg)
{
    int i = 0;
    int start = 0;
    int end = 0;
    char c1 = 0;
    char c2 = 0;
    enode2_t* enode = NULL; 

    char vex[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    char edges[][2] = { 
        {'A', 'G'}, 
        {'A', 'D'},
        {'G', 'A'},
        {'B', 'A'},
        {'B', 'D'},
        {'C', 'F'},
        {'C', 'G'},
        {'D', 'E'},
        {'D', 'F'}
    };

    int vex_num = sizeof(vex) / sizeof(vex[0]);

    int edge_num = sizeof(edges) / sizeof(edges[0]);

    lg->vex_num = vex_num;
    lg->edge_num = edge_num;

    lg->vex = calloc(lg->vex_num, sizeof(vnode_t));

    if(lg->vex == NULL)
    {
        printf("calloc failed!\n");
        return -1;
    }

    for(i = 0; i < (int)lg->vex_num; i++)
    {
        lg->vex[i].data = vex[i];
        lg->vex[i].first_edge = NULL;
    }


    for(i = 0; i < (int)lg->edge_num; i++)
    {
        c1 = edges[i][0];
        c2 = edges[i][1];
        start = get_vex_pos2(lg, c1);
        end = get_vex_pos2(lg, c2);
        enode = (enode2_t*)malloc(sizeof(enode_t));
        enode->ivex = end;
        enode->next_edge = lg->vex[start].first_edge;
        lg->vex[start].first_edge = enode;
    }
    return 0;

}

void DFS(list_graph_t* lg, int index, int* visited)
{
    printf("%c", lg->vex[index].data);
    printf("---->");
    visited[index] = 1;   //该顶点被访问
    enode_t* enode = lg->vex[index].first_edge;
    while(enode != NULL)
    {
        if(visited[enode->ivex] == 0)
        {
            DFS(lg, enode->ivex, visited);
        }
        //相当于寻找当前顶点的邻接顶点
        enode = enode->next_edge;
    }
    //当前顶点没有其余邻接顶点时，该限定条件为递归基
    return;
}
void dfs_traverse(list_graph_t* lg)
{
    int i = 0;
    int* visited = NULL;
    visited = calloc(lg->vex_num, sizeof(int));
    for(i = 0; i < (int)lg->vex_num; i++)
    {
        visited[i] = 0;  //未被访问过的节点为0
    }
    printf("DFS:\n");
    for(i = 0; i < (int)lg->vex_num; i++)
    {
        if(visited[i] == 0) //如果该顶点没有被访问过，那么马上对其进行DFS
        {
            DFS(lg, i, visited);
        }
    }
    free(visited);
    visited = NULL;
}
void BFS(list_graph_t* lg, queue_t* queue, int* visited, int index)
{
    queue->array[queue->write] = index;
    queue->write++;
    int i = 0;
    /* enode_t* enode = lg->vex[index].first_edge; */
    enode_t* enode = NULL;
    //将该节点的所有邻接节点加入到队列中
    while(queue->read != queue->write)
    {
        i = queue->array[queue->read];
        queue->read++;
        visited[i] = 1;
        //访问这个结点
        printf("%c", lg->vex[i].data);
        printf("---->");
        enode = lg->vex[i].first_edge;
        //将所有邻接节点加入到辅助队列中
        while(enode != NULL)
        {
            if(visited[enode->ivex] == 0)
            {
                queue->array[queue->write] = enode->ivex;
                queue->write++;
            }
            enode = enode->next_edge;
        }
    }
}
void bfs_traverse(list_graph_t* lg)
{
    int* visited = NULL;
    visited = calloc(lg->vex_num, sizeof(int));
    queue_t queue;   //辅助队列
    int i = 0;
    queue.array = (int*)calloc(lg->vex_num, sizeof(int));
    queue.read = 0;
    queue.write = 0;
    printf("BFS:\n");
    for(i = 0; i < (int)lg->vex_num; i++)
    {
        visited[i] = 0;
    }

    for(i = 0; i < (int)lg->vex_num; i++)
    {
        if(visited[i] == 0)
        {
            BFS(lg, &queue, visited, i);
        }
    }
    free(queue.array);
    queue.array = NULL;
}
//拓扑排序针对有向无环图
int topology_sort_v1(list_graph_t* lg)
{
    queue_t queue;
    queue.array = calloc(lg->vex_num, sizeof(int));
    queue.read = 0;
    queue.write = 0;
    int* in_degree = NULL;   //入度数组
    in_degree = calloc(lg->vex_num, sizeof(int));
    enode_t* enode = NULL;
    int index = 0;
    char* topology_sort = NULL;
    topology_sort = calloc(lg->vex_num, sizeof(char));
    int offset = 0;
    int i = 0;
    //统计各个顶点的入度
    for(i = 0; i < lg->vex_num; i++)
    {
        enode = lg->vex[i].first_edge;
        while(enode != NULL)
        {
            in_degree[enode->ivex]++;
            enode = enode->next_edge;
        }
    }
    //将入度为0的节点加入到队列中
    for(i = 0; i < lg->vex_num; i++)
    {
        if(in_degree[i] == 0)
        {
            queue.array[queue.write] = i;
            queue.write++;
        }
    }
    while(queue.read != queue.write)
    {
        index = queue.array[queue.read];
        queue.read++;
        topology_sort[offset] = lg->vex[index].data;
        offset++;
        //更改取出节点的邻接节点的入度值
        enode = lg->vex[index].first_edge;
        while(enode != NULL)
        {
            if(in_degree[enode->ivex] != 0)
            {
                in_degree[enode->ivex]--;
            }
            else
            {
                printf("error index:%d\n adjency:%d\n", index, enode->ivex);
            }

            if(in_degree[enode->ivex] == 0)
            {
                queue.array[queue.write] = enode->ivex;
                queue.write++;
            }
            enode = enode->next_edge;
        }
    }

    printf("输出拓扑排序的结果:\n");
    for(i = 0; i < lg->vex_num; i++)
    {
        printf("%c", topology_sort[i]);
        printf("---->");
    }
    free(queue.array);
    queue.array = NULL;
    free(topology_sort);
    topology_sort = NULL;
    free(in_degree);
    in_degree = NULL;
    printf("\n");
    return 0;
}
//拓扑排序的另一种方法，基于DFS， 针对某个顶点的深度优先搜索，会在某个出度为0
//的节点结束，这个出度为0的节点在拓扑排序的结果中应该位于后面(入度为0在前面)
int tsort(list_graph2_t* lg, int index, int* status, stack_t* ps)
{
    //先设置为DISCOVERED
    status[index] = DISCOVERD;
    int i = 0;
    int ret = 0;
    enode2_t* enode = lg->vex[index].first_edge;
    while(enode != NULL)
    {
        switch(status[enode->ivex])
        {
        case UNDISCOVERD:
            
            ret = tsort(lg, enode->ivex, status, ps);
            //如果出现环
            if(ret == -1)
            {
                goto ERR;
            }
            break;
        case DISCOVERD:
            //假设A----->B B----->A A与B之间存在环路, 现在在顶点A处开始执行DFS
            //，A的status被设置为DISCOVERD, B是A的邻接点，现在对B进行DFS，B的
            //status被设置为DISCOVERED，A同时也是B邻接点，对B进行DFS, 会搜索到
            //A，这时A为DISCOVERD
            //出现环了
            enode->property = BACK;
            ret = -1;
            goto ERR;
            break;
        default:
            break;
        }
        enode = enode->next_edge;
    }
    ps->array[ps->top + 1] = lg->vex[index].data;
    ps->top++;
    status[index] = VISITED;
ERR:
    return ret;
}
int topology_sort_v2(list_graph2_t* lg)
{
    stack_t stack;
    stack.array = calloc(lg->vex_num, sizeof(int));
    //现在栈为空
    stack.top = -1;
    int i = 0;
    int ret = 0;
    int* status = calloc(lg->vex_num, sizeof(int));
    for(i = 0; i < lg->vex_num; i++)
    {
        status[i] = UNDISCOVERD;
    }
    for(i = 0; i < lg->vex_num; i++)
    {
        /*该节点尚未被发现 */
        if(status[i] == UNDISCOVERD)
        {
            ret = tsort(lg, i, status, &stack);
            if(ret == -1)
            {
                //出现环立刻结束拓扑排序
                /* break; */
                printf("出现环\n");
                goto ERR;
            }

        }
    }
    printf("输出拓扑排序结果:\n");
    for(i = stack.top; i >=0; i--)
    {
        printf("%c", stack.array[i]);
        printf("---->");
    }
    printf("\n");
    free(stack.array);
    stack.array = NULL;
    stack.top = -1;
    free(status);
    status = NULL;

    return ret;
ERR:
    free(stack.array);
    stack.array = NULL;
    stack.top = -1;
    free(status);
    status = NULL;
    return ret;
}
int main(int argc, char* argv[])
{
    list_graph_t list_graph;
    list_graph2_t list_graph2;
    /* create_example_list_graph(&list_graph); */
    create_example_list_graph2(&list_graph2);
    /* bfs_traverse(&list_graph); */
    /* printf("\n"); */
    /* dfs_traverse(&list_graph); */
    /* printf("\n"); */
    /* topology_sort_v1(&list_graph); */

    topology_sort_v2(&list_graph2);
    return 0;
}
