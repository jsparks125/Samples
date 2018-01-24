#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int LarrysArray()
{
	int T;
	std::cin >> T;
	for (int t = 0; t < T; t++)
	{
		int N;
		std::cin >> N;
		std::vector<int> permutations(N + 1);
		for (int n = 1; n <= N; n++)
			std::cin >> permutations[n];

		for (int i = 1; i < N - 1; i++)
		{
			if (i == permutations[i])
				continue;

			int temp_element = permutations[i];
			for (int j = i; j < N; j++)
			{
				if (permutations[j + 1] == i)
				{
					permutations[j + 1] = temp_element;
					permutations[i] = i;
					if (((j + 1) - i) % 2 == 1)
					{
						int temp_swap_element = permutations[i + 1];
						permutations[i + 1] = permutations[i + 2];
						permutations[i + 2] = temp_swap_element;
					}
					break;
				}
				int new_temp_element = permutations[j + 1];
				permutations[j + 1] = temp_element;
				temp_element = new_temp_element;
			}
		}
		if (permutations[N - 1] != (N - 1))
			std::cout << "NO" << std::endl;
		else
			std::cout << "YES" << std::endl;
	}
	return 0;
}