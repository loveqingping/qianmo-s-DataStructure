/**
 *  Filename: red_black.h
 *   Created: 2017-01-04 15:26:02
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#ifndef RED_BLACK_H
#define RED_BLACK_H
#define RED 0
#define BLACK 1
typedef struct __rb_tree_node
{
    int key;
    struct __rb_tree_node* parent;
    struct __rb_tree_node* left;
    struct __rb_tree_node* right;
    int color;
    /* 用于维护时间链表 */
    struct __rb_tree_node* next_time;
    struct __rb_tree_node* prev_time;
}rb_tree_node_t;

typedef struct __rb_tree
{
    rb_tree_node_t* root;   //指向树根
    rb_tree_node_t* nil;    //指向nil节点
    unsigned long node_num;  //树中的节点数
    rb_tree_node_t* oldest;  //指向最先加入树中的节点
    rb_tree_node_t* latest;  //指向树中最新的节点
}rb_tree_t;
#endif
