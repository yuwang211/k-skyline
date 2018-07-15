#include "kskyline.h"

int main()
{
	freopen("in.txt", "r", stdin);
	int n, k;
	scanf("%d %d", &n, &k);
	KSkyline s(k);
	for (int i = 1; i <= n; ++i)
	{
		int x, y;
		scanf("%d %d", &x, &y);
		printf("Time %d:\n", i);
		printf("\tInsert point (%d, %d)\n", x, y);
		s.insert(x, y);
		while (s.report(x, y))
			printf("\tPoint(%d %d) is removed\n", x, y);
	}
	return 0;
}