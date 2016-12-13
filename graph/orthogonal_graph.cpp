/**
 *  Filename: graph.cpp
 *   Created: 2016-12-06 15:22:21
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct __arcbox
{
    int head_vex;
    int tail_vex;
    struct __arcbox* hlink;
    struct __arcbox* tlink;
}arcbox_t;
typedef struct __vexnode
{
    int data;
    arcbox_t* first_in;
    arcbox_t* first_out;
}vexnode_t;


typedef struct __graph
{
    vexnode_t list[100];
    int vexnode_num;
    int arcbox_num;
}graph_t;

int local_vex(int data, graph_t* g)
{
    int i = 0;
    int ret = -1;
    for(i = 0; i < g->vexnode_num; i++)
    {
        if(data == g->list[i].data)
        {
            ret = i;
            break;
        }
    }
    return ret;
}


int graph_create(graph_t* g)
{
    int x, y;
    int i = 0;
    int j = 0;
    int m = 0;
    int n = 0;
    printf("请输入顶点的个数和边的数量:");
    scanf("%d%d",&g->vexnode_num, &g->arcbox_num);

    for(i = 0; i < g->vexnode_num; i++)
    {
        // scanf("%d", &g->list[i].data);
        g->list[i].data = 30 + i;
        g->list[i].first_out = NULL;
        g->list[i].first_in =  NULL;
    }

    for(j = 0; j < g->arcbox_num; j++)
    {
        // printf("读入(vi-vj):");
        // scanf("%d%d",&x, &y);
        x = 30;
        y = x + j + 1;
        arcbox_t *a = (arcbox_t*)malloc(sizeof(arcbox_t));
        m = local_vex(x, g);
        n = local_vex(y, g);

        a->head_vex = m;
        a->tail_vex = n;

        a->hlink = g->list[m].first_out ;

        a->tlink = g->list[n].first_in;

        g->list[m].first_out = a;

        g->list[n].first_in = a;
    }
}

int graph_display(graph_t* g, int key)
{
    int i = 0;
    int index = -1;
    vexnode_t* v = NULL;
    arcbox_t* a = NULL;

    arcbox_t* b = NULL;
    for(i = 0; i < g->vexnode_num; i++)
    {
        if(g->list[i].data == key)
        {
            index = i;
            break;
        }
    }
    if(index != -1)
    {
        v = &g->list[index];
        a = v->first_out;

        b = v->first_in;
        printf("%d out\n", v->data);
        while(a != NULL)
        {
            printf("%d--->%d\n", g->list[a->head_vex].data, g->list[a->tail_vex].data);
            a = a->hlink;
        }


        printf("%d in\n", v->data);

        while(b != NULL)
        {
            printf("%d--->%d\n", g->list[b->head_vex].data, g->list[b->tail_vex].data);
            b = b->tlink;
        }
        return 0;
    }
    else
    {
        return -1;
    }

}

int main(int argc, char* argv[])
{
    graph_t g;
    graph_create(&g);
    graph_display(&g, 30);
    return 0;
}
