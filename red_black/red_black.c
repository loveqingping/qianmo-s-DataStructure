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
        return T->nil;
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
        //与之前分支对应
        else if(z->parent == z->parent->parent->right)
        {
            //叔节点
            y = z->parent->parent->left;
            //case1:
            if(y->color == RED)
            {
                //父亲节点变为黑色
                z->parent->color = BLACK;
                //叔叔节点变为黑色
                y->color = BLACK;
                //祖父节点变为红色
                //相当于向上层又插入新的节点
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            //case2:
            else
            {
                if(z == z->parent->left)
                {
                    z = z->parent;
                    rb_tree_right_rotate(T, z);
                }
                //case3:
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_tree_left_rotate(T, z->parent->parent);
                //调整结束
            }

        }

    }
    T->root->color = BLACK;
}
#if 0
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
    if(pnode != T->nil)
    {
        //如果有，则将新申请的节点释放
        rb_tree_release_node(&z);
        return -1;
    }
    //如果没有
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
        //根节点的父亲节点为T->nil
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
#endif
int rb_tree_insert(rb_tree_t* T, int key)
{
    rb_tree_node_t* x = T->root;
    rb_tree_node_t* y = T->nil;
    rb_tree_node_t* z = NULL;
    int ret = 0;
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
        else 
        {
            //如果树中已经包含了该节点
            ret = -1;
            goto ERR;
        }
    }
    //alloc a node
    z = rb_tree_alloc_node(key, T->nil);
    if(z == NULL)
    {
        goto ERR;
    }
    //y即为z的父节点
    z->parent = y;
    if(y == T->nil)
    {
        //根节点的父亲节点为T->nil
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
    return ret;
ERR:
    return ret;
}

void rb_transplant(rb_tree_t* T, rb_tree_node_t* u, rb_tree_node_t* v)
{
    if(u->parent == T->nil)
    {
        T->root = v;
    }
    else if(u == u->parent->left)
    {
        u->parent->left  = v;
    }
    else
    {
        u->parent->right = v;
    }
    v->parent = u->parent;
}
rb_tree_node_t* rb_tree_successor(rb_tree_t* T, rb_tree_node_t* z)
{
    rb_tree_node_t* ret = NULL;
    if( (z == T->nil) || (z == NULL) )
    {
        return T->nil;
    }
    else
    {
        while(z != T->nil)
        {
            ret = z;
            z = z->left;
        }
        return ret;
    }
}
#if 0
void rb_tree_delete_fixup(rb_tree_t* T, rb_tree_node_t* x)
{
    while(x != T->root && x->color == BLACK)
    {
        if(x == x->parent->left)
        {
            //x的兄弟
            w = x->parent->right;
            //case1: p节点与w节点颜色互换, 让w颜色变为黑色
            //因为w为红色,所以w的左右孩子均为黑色, 调整结束之后，w的左右孩子会
            //成为x的兄弟，这样x的兄弟的颜色为黑色，这样就转变为其他处理方式
            if(w->color == RED)
            {
                rb_tree_left_rotate(T, x->parent);
                w->color = BLACK;
                x->parent->color = RED;
                w = x->parent->right;
            }
            //case2:
            //     p
            //    / \
            //   x   w
            //  1 2 3 4
            //隐含条件是w为黑色 
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }//w为黑色，右孩子为黑色，左孩子为红色
            else 
            {
                if(w->right->color == BLACK)
                {
                    //因为w->left->color 为红色,可以将其变为黑色，来增加黑节点数目
                    //，从而使全树黑高度平衡
                    w->left->color = BLACK;
                    w->color = RED;
                    rb_tree_right_rotate(T, w);
                    //右旋之后，更新w
                    w = x->parent->right;
                }
                //case4: w的右孩子颜色为红色
                //为了与调整之前保持一致，即将出现在节点p所在位置的节点w，颜色
                //需要与节点p保持一致
                w->color = x->parent->color;
                //开始左旋
                rb_tree_left_rotate(T, x->parent);
                x->parent->color = BLACK;
                w->right->color = BLACK; //再将之前的Old W重新染为黑色
                
                //调整结束, 将x设置为T->root, 终止循环
                x = T->root;
            }
            
        }
        else
        {
            w = x->parent->left;
            if(w->color == RED)
            {
                //将w颜色与p节点的颜色进行互换
                w->color = BLACK;
                x->parent->color =  RED;
                rb_tree_right_rotate(T, x->parent);
                //更新w
                w = x->parent->right;
            }
            //w为黑色, 且其左右孩子均为黑色
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                //w为黑色，右孩子为红色
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    rb_tree_left_rotate(T, w);
                    w->color = RED;
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                rb_tree_right_rotate(T, x->parent);
                x->parent->color = BLACK;
                w->left->color = BLACK;   //Old w改为黑色
                //至此，调整结束
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}
int rb_tree_delete_internal(rb_tree_t* T, rb_tree_node_t* z)
{
    //当z有一个孩子或0个孩子时
    //要被删除的结点就是z本身，所以y等于z
    //始终保持y指向要被删除的节点或移动到树内的节点
    //x始终指向被删除节点的替代节点
    rb_tree_node_t* y = NULL;
    rb_tree_node_t* x = NULL;

    int y_ori_color = 0;
    if(z->left == T->nil)
    {
        y = z;
        //保存y原来的颜色
        y_ori_color = y->color;
        x = y->right;
        rb_transplant(T, y, x);
    }
    else if(z->right == T->nil)
    {
        y = z;
        //保存y原来的颜色
        y_ori_color = y->color;
        x = y->left;
        rb_transplant(T, y, x);
    }
    //z有两个孩子
    else
    {
        y = rb_tree_successor(T, z);
        if(y == T->nil)
        {
            goto ERR;
        }
        else
        {
            y_ori_color = y->color;
            x = y->right;
            //y == z->right
            if(y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                rb_transplant(T, y, x);
                y->right = z->right;
                z->right->parent = y;
            }
            y->left = z->left;
            z->left->parent = y;
            y->color = z->color; //经过这几步，y已经完全替代z，除了数值不一样，颜色，parent, 左右孩子完全相同
            //这几步相当于在树中，将y原先指向的节点删除
            rb_transplant(T, z, y);
            if(y_ori_color == BLACK)
            {
                rb_tree_delete_fixup(T, x);
            }
            //将z释放
            rb_tree_release_node(&z);
        }
    }
ERR:
    return -1;
}
#endif
void rb_tree_delete_fixup(rb_tree_t* T, rb_tree_node_t* x, rb_tree_node_t* parent)
{

    /*************************************
     *   *  maintain red-black tree balance  *
     *       *  after deleting node X            *
     *           *************************************/
    rb_tree_node_t* w = NULL;
    while(x != T->root && x->color == BLACK)
    {
        if(x == parent->left)
        {
            //x的兄弟
            w = parent->right;
            //case1: p节点与w节点颜色互换, 让w颜色变为黑色
            //因为w为红色,所以w的左右孩子均为黑色, 调整结束之后，w的左右孩子会
            //成为x的兄弟，这样x的兄弟的颜色为黑色，这样就转变为其他处理方式
            if(w->color == RED)
            {
                w->color = BLACK;
                parent->color = RED;
                rb_tree_left_rotate(T, parent);
                w = parent->right;
            }
            //case2:
            //     p
            //    / \
            //   x   w
            //  1 2 3 4
            //隐含条件是w为黑色 
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = parent;
                parent = x->parent;
            }//w为黑色，右孩子为黑色，左孩子为红色
            else 
            {
                if(w->right->color == BLACK)
                {
                    //因为w->left->color 为红色,可以将其变为黑色，来增加黑节点数目
                    //，从而使全树黑高度平衡
                    w->left->color = BLACK;
                    rb_tree_right_rotate(T, w);
                    //右旋之后，更新w
                    w = parent->right;
                }
                //case4: w的右孩子颜色为红色
                //为了与调整之前保持一致，即将出现在节点p所在位置的节点w，颜色
                //需要与节点p保持一致
                w->color = parent->color;
                parent->color = BLACK;
                w->right->color = BLACK; //再将之前的Old W重新染为黑色
                //开始左旋
                rb_tree_left_rotate(T, parent);


                //调整结束, 将x设置为T->root, 终止循环
                x = T->root;
            }

        }
        else
        {
            w = parent->left;
            if(w->color == RED)
            {
                //将w颜色与p节点的颜色进行互换
                w->color = BLACK;
                parent->color =  RED;
                rb_tree_right_rotate(T, parent);
                //更新w
                w = parent->left;
            }
            //w为黑色, 且其左右孩子均为黑色
            if(w->left->color == BLACK && w->right->color == BLACK)
            {
                w->color = RED;
                x = parent;
                parent = x->parent;
            }
            else
            {
                //w为黑色，右孩子为红色
                if(w->left->color == BLACK)
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    rb_tree_left_rotate(T, w);
                    w = parent->left;
                }
                w->color = parent->color;
                parent->color = BLACK;
                w->left->color = BLACK;   //Old w改为黑色
                rb_tree_right_rotate(T, parent);
                //至此，调整结束
                x = T->root;
            }
        }
    }
    x->color = BLACK;
}
int rb_tree_delete_internal(rb_tree_t* T, rb_tree_node_t* z)
{
    //当z有一个孩子或0个孩子时
    //要被删除的结点就是z本身，所以y等于z
    //始终保持y指向要被删除的节点或移动到树内的节点
    //x始终指向被删除节点的替代节点
    rb_tree_node_t* y = NULL;
    rb_tree_node_t* x = NULL;
    rb_tree_node_t* parent = NULL;
    int ret = 0;
    if(z == NULL || z == T->nil)
    {
        ret = -1;
        goto ERR;
    }
    if(z->left == T->nil || z->right == T->nil)
    {
        y = z;
    }
    else
    {
        y = z->right;
/* find tree successor with a NIL node as a child */
        while(y->left != T->nil)
        {
            y = y->left;
        }
    }
    if(y->left != T->nil)
    {
        x = y->left;
    }
    else
    {
        x = y->right; //may be NIL!
    }
    /* remove Y from the parent chain */
    parent = y->parent;
    if(x != T->nil)
    {
        x->parent  = parent;
    }
    if(parent != T->nil)
    {
        if(y == parent->left)
        {
            parent->left = x;
        }
        else
        {
            parent->right = x;
        }
    }
    //假设z只有一个孩子，y = z 而且z为根节点，则会进入此分支
    //y的父节点为nil节点, 则y为根节点
    else
    {
        T->root = x;
    }
    if(y != z)
    {
        /*
         *       *  Move the child's data to here, and then
         *               *  re-balance the tree.
         *                       */
        z->key = y->key;
    }
    //z只有一个孩子
    else
    {

    }
    if(y->color == BLACK && x != T->nil)
    {
        rb_tree_delete_fixup(T, x, parent);
    }
    free(y);
    y =  NULL;
    return ret;

ERR:
    return ret;
}

int rb_tree_delete(rb_tree_t* T, int key)
{
    int ret = 0;
    rb_tree_node_t* node = rb_tree_search(T, key);
    //如果树中无这个关键字
    if(node == T->nil)
    {
        ret = -1;
        goto ERR;
    }
    rb_tree_delete_internal(T, node);
    return ret;
ERR:
    return ret;
}


