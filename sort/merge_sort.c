/**
 *  Filename: merge_sort.c
 *   Created: 2016-03-31 09:02:48
 *      Desc: TODO (some description)
 *    Author: du.shangkun(qianmo du), du.shangkun@ampthon.com
 *   Company: Tianyuan Tetong
 */
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
void merge(int* a, int lo, int mi, int hi)
{
    int* temp = calloc(mi - lo, sizeof(int));
    if(temp == NULL)
    {
        return;
    }
    memcpy(temp, &a[lo], (mi - lo) * sizeof(int));
    int i = lo;
    int j = mi;

    int m = 0;
    while(m < mi - lo && j < hi)
    {
        /* if( ((temp[m] <= a[j]) && (m < mi - lo)) || j >= hi)    //如果这里不对m和j的值，做限制，那么m和j会越界,越界之后的a[m]可能为0, 可能对判断结果产生影响 */
        /* { */
            /* a[i] = temp[m]; */
            /* i++; */
            /* m++; */
        /* } */
        /* if( ((temp[m] > a[j] ) && (j < hi)) || m >= mi - lo ) */
        /* { */
            /* a[i] = a[j]; */
            /* i++; */
            /* j++; */
        /* } */
        if(temp[m] < a[j])
        {
            a[i] = temp[m];
            i++;
            m++;
        }
        else
        {
            a[i] = a[j];
            i++;
            j++;
        }
    }
    if(m >= mi - lo)
    {
        while(j < hi)
        {
            a[i] = a[j];
            i++;
            j++;
        }
    }
    if(j >= hi)
    {
        while(m < mi - lo)
        {
            a[i] = temp[m];
            i++;
            m++;
        }
    }
    free(temp);
    temp = NULL;
    return ;
}
void merge_sort(int* a, int lo, int hi)
{
    if(lo >= hi)
    {
        return;
    }
    if(hi - lo < 2)
    {
        return;
    }
    int mi = (hi + lo) >> 1;  //取中值
    merge_sort(a, lo, mi);
    merge_sort(a, mi, hi);
    merge(a, lo, mi, hi);
    return;
}
int main(int argc, char* argv[])
{
    int a[] = {1, 7, 9, 3, 3, 8, 5, 10, 17};
    merge_sort(a, 0, 9);
    int i  = 0;
    for(i = 0; i < 9; ++i)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    return 0;
}
