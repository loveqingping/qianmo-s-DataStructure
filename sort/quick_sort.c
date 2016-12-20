/**
 *  Filename: quick_sort.c
 *   Created: 2016-12-20 15:01:31
 *      Desc: TODO (some description)
 *    Author: dusk(qianmo du), dusk@ifeng.com
 *   Company: Ifeng
 */
#include <stdio.h>
#include <stdlib.h>

int quick_partion(int* elem, int lo, int hi)
{
    //随机选取轴点元素
    /* int temp = elem[lo + rand()%(hi - lo + 1) - 1]; */
    /* elem[lo] = temp ^ elem[lo]; */
    /* temp = elem[lo] ^ temp; */
    /* elem[lo] = elem[lo] ^ temp; */
    int pivot = elem[lo];
    //快速排序的核心在于  将首元素保存到一个变量pivot中，那么整个待排序区就多
    //出一个多余的存储单元(elem[0])，然后按照快速排序分制的思想，从待排序区右
    //边开始寻找小于轴点的元素，找到后，将其放置在lo指向的存储单元，那么lo存储
    //单元的数据被覆盖掉了，但是不需为此担心，刚开始时lo指向的就是轴点元素，而
    //轴点元素已经存储到了pivot中，那么此时待排序区共存储着两份elem[hi]元素，
    //一个存在位置hi处，另一个存在lo处，之后开始移动lo，在[lo, mi]处寻找一个大
    //于pivot的元素然后将其覆盖到elem[hi]处, 最后lo，hi会相等，那么elem[lo]处
    //存放轴点元素

    while(lo < hi)
    {
        while( (lo < hi) )
        {
            //严格大于
            if(elem[hi] > pivot)
            {
                hi--;
            }
            //只要小于等于就赋值给elem[lo]
            else
            {
                elem[lo] = elem[hi];
                lo++;
                break;
            }
        }
        while( (lo < hi))
        {
            if(elem[lo] < pivot)
            {
                lo++;
            }
            else
            {
                elem[hi] = elem[lo];
                hi--;
                break;
            }
        }
    }
    elem[lo] = pivot;
    return lo;
}

void quick_sort(int* elem, int lo, int hi)
{
    int i = 0;
    //单元素区间
    if(hi - lo < 1)
    {
        return;
    }
    int mi = quick_partion(elem, lo, hi);
    printf("mi == %d\n", mi);
    quick_sort(elem, lo, mi);
    quick_sort(elem, mi + 1, hi);
    return ;
}

int main(int argc, char* argv[])
{

    int i = 0;
    int a[10] = { 1, 9, 80, 178, 788, 1980, 144, 2, 3, 16};
    for(i = 0; i < 10; i++)
    {
        printf("%d ", a[i]);
    }


    printf("\n");
    printf("sorted:\n");
    quick_sort(a, 0, 9);
    for(i = 0; i < 10; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

}
