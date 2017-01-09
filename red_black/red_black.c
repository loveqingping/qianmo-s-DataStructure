/**
 *  Filename: red_black.c
 *   Created: 2017-01-04 17:17:35
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include "red_black.h"
#include <malloc.h>
rb_tree_node_t* rb_tree_alloc_node(int key, rb_tree_node_t* init_para)
{
    rb_tree_node_t* pnode = NULL;
    pnode = malloc(sizeof(rb_tree_node_t));
    if(pnode == NULL)
    {
        return NULL;
    }
    pnode->key = -1;
    pnode->right = init_para;
    pnode->left = init_para;
    pnode->parent = init_para;
    return pnode;
}
int rb_tree_release_node(rb_tree_node_t** ppnode)
{
    if(ppnode ==  NULL)
    {
        return -1;
    }
    rb_tree_node_t* pnode = *ppnode;
    if(pnode ==  NULL)
    {
        return -1;
    }
    
    free(pnode);
    *ppnode = NULL;
    return 0;
}
rb_tree_t* rb_tree_create()
{
    rb_tree_t* T = malloc(sizeof(rb_tree_t));
    if(T == NULL)
    {
        return NULL;
    }
    T->nil = rb_tree_alloc_node(-1, NULL);
    if(T->nil == NULL)
    {
        return NULL;
    }
    T->nil->color = BLACK;  //因为是nil节点
    T->root = T->nil;   //树刚开始时的形态
    return T;
}
rb_tree_node_t* rb_tree_internal_search(rb_tree_t* T, rb_tree_node_t* x, int key)
{
    while(x != T->nil)
    {
        if(x->key > key)
        {
            x = x->left;
        }
        else if(x->key < key)
        {
            x = x->right;
        }
        //x->key == key
        else
        {
           break; 
        }
    }
    return x;
}
rb_tree_node_t* rb_tree_search(rb_tree_t* T, int key)
{
    //树不为空
    if(T->root != T->nil)
    {
        return rb_tree_internal_search(T, T->root, key);
    }
    //树为空
    else
    {
        return NULL;
    }
}
void rb_tree_left_rotate(rb_tree_t* T, rb_tree_node_t* x)
{
    rb_tree_node_t* y = NULL;
    y = x->right;
    x->right = y->left;
    //y的左孩子为空
    if(y->left != T->nil)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;

    //根节点的父节点为nil节点
    if(x->parent == T->nil)
    {
        T->root = y;
    }
    //如果x是父节点的左孩子
    else if(x == x->parent->left)
    {
        x->parent->left = y;

    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
/* 
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x                  
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y  
 *       / \                                   / \                   #
 *      lx  rx     			               rx  ry
 * 
 */
void rb_tree_right_rotate(rb_tree_t* T, rb_tree_node_t* x)
{
    rb_tree_node_t* y = NULL;
    y = x->left;
    x->left = y->right;
    if(y->right != T->nil)
    {
        y->right->parent = x;
    }
    //x是根节点
    if(x->parent == T->nil)
    {
        T->root = y;
    }
    //如果x是父节点的左孩子
    else if(x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;
}
void rb_tree_insert_fixup(rb_tree_t* T, rb_tree_node_t* z)
{
    //发生双红缺陷时，树至少有3层
    rb_tree_node_t* y = NULL;
    while(z->parent->color == RED)
    {
        if(z->parent == z->parent->parent->left)
        {
            //叔叔节点y
            y = z->parent->parent->right;
            //case1:
            //需要将红黑树与B树结合到一起进行学习，case1发生时，祖父节点为黑色
            //节点，将z节点，z节点的父节点，z节点的叔节点与祖父节点合并成一个
            //大节点，这个大节点对应B树中的节点，红黑
            //树本质上为4阶B树，因此一个节点的关键字数目不超过3, 由于叔叔节点
            //为红色，导致节点发生上溢
            if(y->color == RED)
            {
                z->parent->color = BLACK;
                y->color = BLACK;
                //祖父节点变红相当于向更上层插入一个新节点
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            //如果叔节点为黑色
            else
            {
                //case2: 需要先变换到case3
                if(z == z->parent->right)
                {
                    z = z->parent;
                    rb_tree_left_rotate(T, z);
                }
                //case3:
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_tree_right_rotate(T, z->parent->parent);
            }

        }
        else if(z->parent == z->parent->parent->right)
        {

        }

    }
    T->root->color = BLACK;
}
int rb_tree_insert(rb_tree_t* T, int key)
{
    //下面语句包含了将z->parent = T->nil
    rb_tree_node_t* z = rb_tree_alloc_node(key, T->nil);
    rb_tree_node_t* x = T->root;
    rb_tree_node_t* y = T->nil;
    rb_tree_node_t* pnode = NULL;
    if(z == NULL)
    {
        return -1;
    }
    //先在树中查找是否有相同节点
    pnode = rb_tree_search(T, key);
    if(pnode != NULL)
    {
        //如果有，则将新申请的节点释放
        rb_tree_release_node(&z);
        return -1;
    }
    while(x != T->nil)
    {
        y = x;
        if(x->key > key)
        {
            x = x->left;
        }
        else if(x->key < key)
        {
            x = x->right;
        }
    }
    //y即为z的父节点
    z->parent = y;
    if(y == T->nil)
    {
        T->root = z;
    }
    if(y->key > z->key)
    {
        y->left = z;
    }
    else if(y->key < z->key)
    {
        y->right = z;
    }
    z->right = T->nil;
    z->left = T->nil;
    z->color = RED;
    rb_tree_insert_fixup(T, z);
    return 0;
}


