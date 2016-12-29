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
			/*���ִ���*/
			return NULL;
		}
	}
	int j = 0;
	int t = next_array[0] = -1;
	//t����next�����е�ֵ

	/*�������� :  if(p[j] == p[next[j]]) {next[j + 1] = next[j] + 1;}*/

	//��������ļ������ݣ����j ֻ�������� len - 2,����

	while (j < len - 1)
	{
		/*��Ϊ��kmp�㷨���и��ǳ���Ҫ�ĸ���, ͨ���ڱ���p[-1] �����κ��ַ�ƥ�䶼�ɹ� */

		if (t < 0   || p[j] == p[t])  //t < 0�������Ӧ :��Ϊp[-1] == p[0] next_array[1] = next_array[0] + 1; 
		{
			
			next_array[j + 1] = next_array[j] + 1;
			j++;                              //j�Ѿ�+1
			t = next_array[j];

			if (p[j] == p[t])   // �൱��û���滻��ƥ����ȻʧЧ
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
	if (j == n)   //ƥ��ɹ�
	{
		return &text[i - j];
	}
	else  //ƥ��ʧ��
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