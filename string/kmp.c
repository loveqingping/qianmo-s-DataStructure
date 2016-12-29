#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int* build_next_array(char* p)
{
	int* next_array = NULL;
	int len = strlen(p);
	if (len > 0)
	{
		next_array = (int*)calloc(len, sizeof(int));
		if (next_array == NULL)
		{
			/*出现错误*/
			return NULL;
		}
	}
	int j = 0;
	int t = next_array[0] = -1;
	//t代表next数组中的值

	/*计算依据 :  if(p[j] == p[next[j]]) {next[j + 1] = next[j] + 1;}*/

	//根据上面的计算依据，因此j 只需自增到 len - 2,即可

	while (j < len - 1)
	{
		/*因为在kmp算法中有个非常重要的概念, 通配哨兵，p[-1] 它与任何字符匹配都成功 */

		if (t < 0   || p[j] == p[t])  //t < 0的情况对应 :因为p[-1] == p[0] next_array[1] = next_array[0] + 1; 
		{
			
			next_array[j + 1] = next_array[j] + 1;
			j++;                              //j已经+1
			t = next_array[j];

			if (p[j] == p[t])   // 相当于没有替换，匹配依然失效
			{
				next_array[j] = next_array[t];
			}
		}
		else
		{
			t = next_array[t];
		}
	}
	return next_array;
}

char* kmp_match(char* text, char* pattern)
{
	if (text == NULL || pattern == NULL)
	{
		return NULL;
	}
	int n = strlen(pattern);
	int m = strlen(text);
	int* next = build_next_array(pattern);

	int i = 0;   //text
	int j = 0;   //pattern
	while (i < m && j < n)
	{
		if (j < 0 || pattern[j] == text[i])
		{
			i++;
			j++;
		}
		else
		{
			j = next[j];
		}
	}
	free(next);
	next = NULL;
	if (j == n)   //匹配成功
	{
		return &text[i - j];
	}
	else  //匹配失败
	{
		return NULL;
	}

}

int main(int argc, char* argv[])
{
	char* text = "abcdefg";
	char* p = kmp_match(text, "hijk");

	if (p != NULL)
	{
		printf("%s\n", p);
	}
	getchar();
	return 0;
}