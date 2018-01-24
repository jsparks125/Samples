#include <set>
#include <string>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int AbsolutePermutations()
{
	int t;
	std::cin >> t;
	for (int a0 = 0; a0 < t; a0++)
	{
		int n;
		int k;
		std::cin >> n >> k;
		bool perm_possible = true;
		std::set<int> chosen;
		std::vector<int> permutations;
		for (int i = 1; i <= n; i++)
		{
			int first = (i - k >= 1) ? i - k : -1;
			int second = (i + k <= n) ? i + k : -1;

			if (chosen.find(first) == chosen.end() && first != -1)
			{
				permutations.push_back(first);
				chosen.insert(first);
			}
			else if (chosen.find(second) == chosen.end() && second != -1)
			{
				permutations.push_back(second);
				chosen.insert(second);
			}
			else
			{
				std::cout << -1 << std::endl;
				perm_possible = false;
				break;
			}
		}
		if (perm_possible)
		{
			for (int i = 0; i < permutations.size(); i++)
				std::cout << permutations[i] << " ";
			std::cout << std::endl;
		}
	}
	return 0;
}
