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
//由用户实现
typedef int (*rb_tree_key_cmp_func)(void* key1, void* key2, unsigned int key_size); 
typedef struct __rb_tree_node
{
    // int key;
    struct __rb_tree_node* parent;
    struct __rb_tree_node* left;
    struct __rb_tree_node* right;
    int color;
    /* 用于维护时间链表 */
    struct __rb_tree_node* next_time;
    struct __rb_tree_node* prev_time;
    //用于存储数据
    char key[0];
}rb_tree_node_t;

typedef struct __rb_tree
{
    rb_tree_node_t* root;   //指向树根
    rb_tree_node_t* nil;    //指向nil节点
    unsigned long node_num;  //树中的节点数
    rb_tree_node_t* oldest;  //指向最先加入树中的节点
    rb_tree_node_t* latest;  //指向树中最新的节点
    unsigned int key_size;   //关键码大小
    rb_tree_key_cmp_func cmp;
}rb_tree_t;
rb_tree_t* rb_tree_create(unsigned int key_size, rb_tree_key_cmp_func cmp);
rb_tree_node_t* rb_tree_search(rb_tree_t* T, void* key);
int rb_tree_insert(rb_tree_t* T, void* pkey);
int rb_tree_delete(rb_tree_t* T, void* pkey);
rb_tree_node_t* rb_tree_get_oldest(rb_tree_t* T);
rb_tree_node_t* rb_tree_get_latest(rb_tree_t* T);
unsigned int rb_tree_get_node_num(rb_tree_t* T);
void rb_tree_destroy(rb_tree_t** T);
//只适合调试时使用
int rb_tree_display(rb_tree_node_t* root);

#endif
