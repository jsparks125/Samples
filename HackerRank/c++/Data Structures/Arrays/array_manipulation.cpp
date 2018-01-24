#include <iostream>
#include <vector>

int ArrayManipulation()
{
	int n, m;
	std::cin >> n >> m;

	std::vector<long> v(n + 1);
	for (int i = 0; i < m; i++)
	{
		int a, b, k;
		std::cin >> a >> b >> k;
		v[a] += k;
		if (b + 1 <= n)
			v[b + 1] -= k;
	}

	long max = 0;
	long sum = 0;
	for (int i = 1; i <= n; i++)
	{
		sum += v[i];
		if (max < sum)
			max = sum;
	}
	std::cout << max;
	return 0;
}
