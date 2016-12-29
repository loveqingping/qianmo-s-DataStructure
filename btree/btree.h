/**
 *  Filename: btree.h
 *   Created: 2016-12-26 10:19:23
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
typedef struct __btree_node
{
    int degree;   //标明该B树的阶数 只支持偶数阶，和算法导论中保持一致
    int t;        //该B树允许的最小分支数 t = degree / 2 
    int n;   //标明该节点存储的key的数量  t - 1 <= n <= 2t - 1 
    int* key;   //array for key
    int leaf;   //叶子节点标记
    struct __btree_node** child;   //array for child
}btree_node_t;
typedef struct __btree
{
    int degree;    //标明该B树的阶数
    int t;        //该B树允许的最小分支数 t = degree / 2 
    btree_node_t* root;   //用来指向B树的树根节点
    int h;                //B树的树高
}btree_t;

