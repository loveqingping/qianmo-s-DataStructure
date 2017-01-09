/**
 *  Filename: btree.c
 *   Created: 2016-12-26 10:19:11
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include "btree.h"
btree_node_t* btree_search(btree_node_t* x, int key, int* seq)
{
    //重点理解B树形成的过程
    //为什么在btree_search函数不担心x ->child[x->n]为空情况
    //要想理解原因，必须要理解B树形成的过程，x->key[n - 1]关键码的两个子树x->child[n-1]，x->child[n]什么时候形成
    
    //假设此时x->child[n]指向的y子节点已满, 该子节点需要进行分裂， x->child[n]
    //指向原来的y x->child[n+1]指向新的z节点(n还未增加所以是n+1)，，在B树中任何内部节点中每个关键码
    //的左右子树必定同时存在
    int i = 0;
    while(i < x->n && x->key[i] < key)
    {
        i++;
    }
    if(i < x->n && (x->key[i] == key) )
    {
        *seq = i;
        return x;
    }
    
    if(x->leaf)
    {
        *seq = -1;
        return NULL;
    }
    else
    {
        btree_search(x->child[i], key, seq);
    }
}
btree_node_t* btree_alloc_node(int degree)
{
    btree_node_t* node = malloc(sizeof(btree_node_t));
    if(node == NULL)
    {
        return NULL;
    }
    node->degree = degree;
    node->t = node->degree / 2;
    node->key = calloc(node->degree - 1, sizeof(btree_node_t));
    if(node->key == NULL)
    {
        return NULL;
    }
    //calloc自动清零
    node->child = calloc(node->degree, sizeof(btree_node_t*));
    if(node->child == NULL)
    {
        return NULL;
    }
    node->n = 0;
    return node;
}
int btree_release_node(btree_node_t** pnode)
{
    btree_node_t* node = *pnode;
    free(node->key);
    node->key = NULL;
    free(node->child);
    node->child = NULL;

    free(node);
    node = NULL;
    *pnode = node;
    return 0;
}
//x:指向一个非满节点
//i:x->child[i]为满节点
void btree_spilt_child(btree_node_t* x, int i)
{
    //y指向满节点
    //z指向一个新节点
    int j = 0;
    int t = x->t;
    btree_node_t* z = btree_alloc_node(x->degree);
    btree_node_t* y = x->child[i];
    z->leaf = y->leaf;

    z->n = t - 1;

    for(j = 0; j < t - 1; j++)
    {
        z->key[j] = y->key[j + t];
    }

    //如果y不是叶子节点，必然有孩子，那么也应该将孩子数组中的指针拷贝到z中
    if(y->leaf != 1)
    {
        for(j = 0; j < t - 1; j++)
        {
            z->child[j] = y->child[j + t];
        }

    }
    //将x中的孩子数组从序号i+1 到序号 x->n 整体向右移动一个单元，腾出指向新孩子z的单元
    //如果i == x->n 那么下面循环不执行
    for(j = x->n; j >= i + 1; j--)
    {
        x->child[j + 1] = x->child[j];
    }
    x->child[i + 1] = z;
    //将x中的关键字数组从序号i 到序号 x->n - 1 整体向右移动一个单元，腾出存储
    //y[t - 1]
    for(j = x->n - 1; j >= i; j--)
    {
        x->key[j + 1] = x->key[j];
    }
    x->key[i] = y->key[t - 1];
    y->n = t - 1;
    //x中关键字数目增加
    x->n++;
}
//B树插入的核心是往叶子节点中插入元素，等待叶子节点成为满节点后，发生分裂，向
//父亲节点献上中间节点，不断如此，根节点会满，这时候就根节点会发生分裂， B树高
//度增加
void btree_insert_nofull(btree_node_t* x, int k)
{
    int j = 0;
    int t = x->t;
    //如果节点x为叶子节点
    if(x->leaf == 1)
    {
        for(j = x->n - 1; j >= 0 && k < x->key[j]; j--)
        {
            x->key[j + 1] = x->key[j];
        }
        x->key[j + 1] = k;
        x->n++;
    }
    //如果x不是叶子节点, 需要插入到以x为根的子树中适当的叶节点中
    else
    {
        //寻找适当的子树
        for( j = x->n - 1; j >= 0 && x->key[j] > k; j--)
        {
        }
        j = j + 1;
        /* 如果k比x中任何关键字都要大，经过上面的循环之后j会等于n 那么就会进入x->child[n]这个分支，但是这 */
        /* 时候x->key[n]是无意义节点(关键字序号最多到n-1), 这时候要借助哨兵的思 */
        /* 想理解，认为x->key[n]存在只是，x->key[n]真值为无穷大, x->child[n]指 */
        /* 向的子节点肯定比x->key[n]要小 */
        
        //如果子节点发生上溢
        if(x->child[j]->n == 2 * t - 1)
        {
            btree_spilt_child(x, j);
            //分裂后x->key[j] 存储以前y中的key[t-1]，需要重新比较一下k与
            //x->key[j]的大小
            if(k > x->key[j])
            {
                j = j + 1;
            }

            btree_insert_nofull(x->child[j], k);
        }
        else
        {
            btree_insert_nofull(x->child[j], k);
        }
    }
}
void btree_insert(btree_t* T, int key)
{
    btree_node_t* r = T->root;
    btree_node_t* s = NULL;
    if(r->n == T->t * 2 - 1)
    {
        s = btree_alloc_node(T->degree);
        T->root = s;
        s->n = 0;
        s->leaf = 0;
        s->child[0] = r;
        //这里可能会有疑问，此时s中还没有存储一个关键字呢，为什么能直接分裂原
        //来的根结点呢，因为这是一种特殊情况，根节点的关键字数目达到上限，必须
        //要分裂根节点，但是此时根节点肯定没有父亲节点，因此需要重新开辟一个节
        //点作为根节点的父亲节点, 只有根节点发生分裂时，B树的高度才会变化
        //分裂之后s->key[0]存储之前root->key[t-1]
        //
        btree_spilt_child(s, 0);
        btree_insert_nofull(s, key);
        T->h++;  //B树高度增加
    }
    else
    {
        btree_insert_nofull(r, key);
    }
}
btree_t* btree_create(int degree)
{
    //只支持偶数阶
    if(degree % 2 != 0)
    {
        return NULL;
    }
    btree_t* T = malloc(sizeof(btree_t));
    if(T == NULL)
    {
        return NULL;
    }
    else
    {
        T->degree = degree;
        T->t = T->degree / 2;
        T->root = btree_alloc_node(T->degree);
        if(T->root == NULL)
        {
            return NULL;
        }
        T->root->leaf = 1;  //最开始的根节点为叶子节点
        T->h = 0;
    }
    return T;
}
void btree_delete(btree_t* T, btree_node_t* x, int k)
{
    int i = 0;
    int j = 0;
    btree_node_t* y = NULL;
    btree_node_t* z = NULL;
    btree_node_t* d = NULL;
    int t = x->t;
    while(i < x->n && (x->key[i] < k) )
    {
        i++;
    }
    //如果找到了
    if(i < x->n && (k == x->key[i]) )
    {
        //如果x是叶子节点, 直接删除即可
        if(x->leaf == 1)
        {
            //覆盖x->key[i]
            for(j = i; j < x->n - 1; j++)
            {
                x->key[j] = x->key[j + 1];

            }
            x->n--;
            return;
        }
        //如果是内部节点
        else
        {

            y = x->child[i];
            z = x->child[i + 1];
            //2a: 如果节点x小于k的子节点y至少包含t个关键字，则找出k在以y为根的
            //子树中的前驱k'
            if(y->n >= t)
            {
                d = y;
                while(d->leaf == 0)
                {
                    //相当于在以y为根的子树中找到最大的节点
                    d = d->child[d->n];
                }
                //替换
                x->key[i] = d->key[d->n - 1];
                //递归删除
                btree_delete(T, y, d->key[d->n - 1]);
            }
            //2b: 如果节点x大于k的子节点z至少包含t个关键字，则找出k在以z为根的
            //子树中的后继k'
            else if(z->n >= t)
            {
                d = z;
                while(d->leaf == 0)
                {
                    //相当于在以Z为根中寻找最小节点
                    d = d->child[0];
                }
                //替换
                x->key[i] = d->key[0];
                btree_delete(T, z, d->key[0]);
            }
            //2c: y和z都只含有t - 1个关键字， 则将k和z的全部合并进y，这样x就失
            //去了k和指向z的指针， 并且y现在包含2*t - 1个关键字，然后释放z并递
            //归地从y中删除k
            else
            {
                //合并y和x
                y->key[y->n] = x->key[i];
                y->n++;
                for(j = i; j < x->n - 1; j++)
                {
                    x->key[j] = x->key[j + 1];
                }
                //合并y和z
                for(j = 0; j < z->n; j++)
                {
                    y->key[y->n + j] = z->key[j];
                }
                //如果y和z不是叶节点, 将孩子数组合并
                if(y->leaf == 0)
                {
                    for(j = 0; j < z->n + 1; j++)
                    {
                    //因为y->n已经增加，所以y->child[y->n]就指向孩子数组的最后
                    //位置
                        y->child[y->n + j] = z->child[j];
                    }
                }
                y->n += z->n;

                //在x的孩子数组中清除指向z的指针
                for(j = i + 1; j <= x->n - 1; j++)
                {
                    x->child[j] = x->child[j + 1];
                }
                x->n--;
                //如果x是根节点
                if( (x->n == 0) && x == T->root)
                {
                    T->root = y;
                    btree_release_node(&x);
                }
                //释放z
                btree_release_node(&z);
                //递归删除y
                btree_delete(T, y, k);
            }
        }
    }
    else
    {
        //3a:如果x->child[i]->n == t - 1 但是它的相邻兄弟至少有t个关键字, 则将
        //x中某个关键字降至x->child[i] 中将x->child[i]的相邻兄弟的， 并且将该
        //关键字相应的孩子指针移到x->child[i]中
        
        //如果x已经是叶节点了，肯定找不到了，直接返回即可
        if(x->leaf == 1)
        {
            return;
        }

        y = x->child[i];
        if(y->n == t - 1)
        {
            //孩子数组序号从0到x->n, 如果i为x->n, 那么相邻兄弟只有一个，只能是
            //x->child[i - 1]
            if(i < x->n && x->child[i + 1]->n >= t )
            {
                z = x->child[i + 1];
                //将x->key[i]元素降至y->key[y->n]
                y->key[y->n] = x->key[i];
                //将z->key[0]元素升至x->key[i]
                x->key[i] = z->key[0];
                //z中关键字减少,需要覆盖

                for(j = 0; j < z->n - 1; j++)
                {
                    z->key[j] = z->key[j + 1];
                }
                //如果z不是叶子节点，还需要处理孩子数组
                if(z->leaf == 0)
                {
                    //z->key[0]已经被覆盖，其子树要重新挂在子树y上
                    y->child[y->n + 1] = z->child[0];
                    for(j = 0; j < z->n; j++)
                    {
                        z->child[j] = z->child[j + 1];
                    }
                }
                y->n++;
                z->n--;
            }
            //孩子数组序号从0到x->n, 如果i为0, 那么相邻兄弟只有一个，只能是
            //x->child[i + 1]

            else if(i > 0 && x->child[i - 1]->n >= t)
            {
                z = x->child[i - 1];
                for(j = y->n - 1; j >= 0; j--)
                {
                    y->key[j + 1] = y->key[j];
                }
                y->key[0] = x->key[i - 1];
                x->key[i - 1] = z->key[z->n - 1];

                if(y->leaf == 0)
                {
                    for(j = y->n; j >= 0; j--)
                    {
                        y->child[j + 1] = y->child[j];
                    }
                    y->child[0] = z->child[z->n];
                }
                z->n--;
                y->n++;
            }
            //3b:如果x->child[i]的所有的相邻兄弟都只包含t-1个关键字，则将两个
            //兄弟合并
            //因为i < x->n 因此x->child[i + 1]必存在, y可以与x->child[i + 1]进
            //行合并
            else if(i < x->n)
            {
                z = x->child[i + 1];
                //下降x->key[i]到y->key[y->n]中
                y->key[y->n] = x->key[i];
                //将x->key[i]覆盖
                for(j = i; j < x->n - 1; j++)
                {
                    x->key[i] = x->key[i + 1];
                }
                y->n++;
                //将y与z合并
                for(j = 0; j < z->n; j++)
                {
                    y->key[y->n + j] = z->key[j];
                }
                //如果y不是叶子节点，还需将z的孩子指针数组复制过来
                if(y->leaf == 0)
                {
                    for(j = 0; j <= z->n; j++)
                    {
                        y->child[y->n + j] = z->child[j];
                    }
                }
                //y, z完成合并
                y->n += z->n; 
                //在x中删除指向z的指针
                for(j = i + 1; j <= x->n - 1; j++ )
                {
                    x->child[j] = x->child[j + 1];
                }
                x->n--;
                //释放z
                btree_release_node(&z);
                //根节点x已空
                if(x->n == 0 && T->root == x)
                {
                    T->root = y;
                    btree_release_node(&x);
                }
            }
            // i == x->n
            else 
            {
                z = x->child[i - 1];
                //将y和z两个节点的内容合并到z中
                z->key[z->n] = x->key[i - 1];
                for(j = i - 1; j < x->n - 1; j++)
                {
                    x->key[j] = x->key[j + 1];
                }
                z->n++;
                for(j = 0; j < y->n; j++)
                {
                    z->key[z->n + j] = y->key[j];
                }
                /* 如果y不是叶子节点 */
                if(y->leaf == 0)
                {
                    for(j = 0; j <= y->n; j++)
                    {
                        z->child[z->n + j] = y->child[j];
                    }
                }
                z->n += y->n;
                // 从x中删除指向y的指针
                for(j = i ; j <= x->n - 1; j++)
                {
                    x->child[j] = x->child[j + 1];

                }
                x->n--;
                if( (x->n == 0) && T->root == x)
                {
                    T->root = z;
                    btree_release_node(&x);
                }
                //释放y
                btree_release_node(&y);
            }
        }
        //调整完成以后，继续删除
        btree_delete(T, y, k);
    }
}

void __btree_print (btree_node_t* x)
{
    int i;
    for (i = 0; i < x->n; i++) {
            if (!x->leaf)
                __btree_print (x->child[i]);
            printf ("%d ", x->key[i]);
        }   
    if (!x->leaf)
        __btree_print (x->child[i]);
}

void btree_print (btree_t* T)
{
    __btree_print (T->root);
}

void __btree_graph (btree_node_t* x)
{
    int i;
    printf ("\"%p\" [shape=box,label=\"", x);
    for (i = 0; i < x->n; i++)
        printf("%d ", x->key[i]);
    printf ("\"];\n");

    for (i = 0; i < x->n; i++) {
        if (!x->leaf) {
            printf ("\"%p\" -> \"%p\";\n", x, x->child[i]);
            __btree_graph (x->child[i]);
        }
    }
    if (!x->leaf) {
        printf ("\"%p\" -> \"%p\";\n", x, x->child[i]);
        __btree_graph (x->child[i]);
    }
}

void btree_graph (btree_t* T)
{
    printf ("digraph graphname {\n");
    __btree_graph (T->root);
    printf ("}\n");
}
