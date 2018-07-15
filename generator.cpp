#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <cstdlib>


int main(int args, char **argv)
{
	int n = atoi(argv[1]);
	int k = atoi(argv[2]);
	freopen(argv[3], "w", stdout);
	printf("%d %d\n", n, k);
	for (int i = 1; i <= n; ++i)
		printf("%d %d\n", rand() % 10, rand() % 10);
	return 0;
}