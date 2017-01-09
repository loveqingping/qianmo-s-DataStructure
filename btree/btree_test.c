/**
 *  Filename: btree_test.c
 *   Created: 2016-12-30 10:03:27
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "btree.h"

int main(int argc, char* argv[])
{
    int i = 0;
    if(argc != 2)
    {
        printf("Usage:./cmd degree");
        exit(-1);
    }

    int count = atoi(argv[1]);

    btree_t* T = btree_create(4);
    srand(time(NULL));

    for(i = 0; i < count; i++)
    {
        btree_insert(T, i);
    }
    btree_delete(T, T->root, 1);
    btree_graph(T);
    return 0;
}
