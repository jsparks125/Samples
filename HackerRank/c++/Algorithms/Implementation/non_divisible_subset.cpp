#include <cmath>
#include <cstdio>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>

int NonDivisbleSubsetMain()
{
	int n, k;
	std::cin >> n >> k;
	std::vector<int> sets(k);
	for (int i = 0; i < n; i++)
	{
		int item;
		std::cin >> item;
		sets[item % k]++;
	}

	int total_sets = 0;
	if (sets[0] > 0)
		total_sets = 1;
	for (int i = 1; i <= sets.size() / 2; i++)
	{
		if (i != (sets.size() - i))
		{
			if (sets[i] > sets[sets.size() - i])
				total_sets += sets[i];
			else
				total_sets += sets[sets.size() - i];
		}
	}
	if (k % 2 == 0)
		total_sets++;

	std::cout << total_sets;
	return 0;
}
