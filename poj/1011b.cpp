#include <stdio.h>

#include <string.h>

#include <stdlib.h>

int n, sum, len, number;

int sticks[65];

bool visit[65], flag;

int cmp(const void *a, const void *b)

{

	return *(int*)b - *(int*)a;

}

//lengthΪ��ǰľ����ϵĳ���,numΪ��ǰ���ľ��������

void dfs(int p, int length, int num)

{

	int i, pre;

	if (num == number)   //����Ҫ��

	{

		flag = true;

	}

	else if (length == len) //����һ������Ѿ�����Ҫ��

	{

		dfs(0, 0, num + 1); //��ǵ�ǰ���ľ������+1,����һ���������

	}

	else

	{

		for (i = p, pre = -1; i < n; i++)

		{

			if (!visit[i] && sticks[i] != pre && sticks[i] + length <= len)

			{

				pre = sticks[i];   //��֦1

				visit[i] = true;

				dfs(i + 1, length + sticks[i], num);

				visit[i] = false;

				if (flag || p == 0) //��֦2

				{

					return;

				}

			}

		}

	}

}

int main()

{

	int i;

	while (scanf("%d", &n) && n)

	{

		sum = 0;

		flag = false;

		for (i = 0; i < n; i++)

		{

			scanf("%d", &sticks[i]);

			sum += sticks[i];

		}

		qsort(sticks, n, sizeof(sticks[0]), cmp); //�Ӵ�С����,����������˳��,������Χ����

		for (len = sticks[0]; len < sum; len++)

		{

			if (sum % len == 0)

			{

				number = sum / len;

				memset(visit, false, sizeof(visit));

				dfs(0, 0, 0);

				if (flag)

				{

					break;

				}

			}

		}

		printf("%d\n", flag ? len : sum);

	}

	return 0;

}