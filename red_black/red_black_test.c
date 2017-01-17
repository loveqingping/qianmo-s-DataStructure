/**
 *  Filename: red_black_test.c
 *   Created: 2017-01-16 15:55:31
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include "red_black.h"
#define LENGTH(a) ( (sizeof(a)) / (sizeof(a[0])) )

int  main(int argc, char* argv[])
{
    int a[] = { 12, 1, 9, 2, 0, 11, 7, 19, 4, 15, 18, 5, 14, 13, 10, 16, 6, 3, 8, 17 };
    /* int a[] = {12, 1, 9, 2}; */
    int i, ilen = LENGTH(a);
    rb_tree_t* T = NULL;

    T = rb_tree_create(4, NULL);
    printf("== 原始数据: ");
    for (i = 0; i < ilen; i++)
        printf("%d ", a[i]);
    printf("\n");

    for (i = 0; i < ilen; i++)
    {
        if(a[i] == 8)
        {
            rb_tree_insert(T, &a[i]);
        }
        else
        {
            rb_tree_insert(T, &a[i]);
        }
        printf("== 添加节点: %d\n", a[i]);
    }
    printf("== 树的详细信息: \n");
    //插入完成
    rb_tree_display(T->root);
    printf("\n");
#if 1
    for (i = 0; i < ilen; i++)
    {
        if (a[i] == 16)
        {
            rb_tree_delete(T, &a[i]);
        }
        else
        {
            rb_tree_delete(T, &a[i]);
        }
        printf("== 删除节点: %d\n", a[i]);
        if(T->latest != NULL)
        {
            printf("当前最新节点:%p %d\n", T->latest, *((int*)T->latest->key) );
        }
        /* if(a[i] == 5) */
        /* { */
            /* if (T->root != T->nil) */
            /* { */
                /* printf("== 树的详细信息: \n"); */
                /* rb_tree_display(T->root); */
                /* printf("\n"); */
            /* } */
        /* } */
    }
#endif
    rb_tree_destroy(&T);
    return 0;
}
