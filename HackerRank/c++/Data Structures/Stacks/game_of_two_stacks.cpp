#include <cmath>
#include <cstdio>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int GameOfTwoStacks() 
{
	int g;
	std::cin >> g;
	for (int a0 = 0; a0 < g; a0++)
	{
		int n;
		int m;
		int x;
		std::cin >> n >> m >> x;
		std::vector<int> a;
		int a_sum = 0;
		bool a_done = false;
		for (int a_i = 0; a_i < n; a_i++)
		{
			int new_a;
			std::cin >> new_a;
			if (!a_done)
			{
				if (a_sum + new_a <= x)
				{
					a.push_back(new_a);
					a_sum += new_a;
				}
				else
					a_done = true;
			}
		}
		std::vector<int> b;
		int b_sum = 0;
		bool b_done = false;
		for (int b_i = 0; b_i < m; b_i++)
		{
			int new_b;
			std::cin >> new_b;
			if (!b_done)
			{
				if (b_sum + new_b <= x)
				{
					b.push_back(new_b);
					b_sum += new_b;
				}
				else
					b_done = true;
			}
		}

		int a_moves = a.size();
		int b_moves = 0;
		int total_moves = a_moves;
		while (b_moves < b.size() && a_moves >= 0)
		{
			a_sum += b[b_moves];
			b_moves++;
			while (a_sum > x && a_moves > 0)
			{
				a_moves--;
				a_sum -= a[a_moves];
			}
			if (a_sum <= x && a_moves + b_moves > total_moves)
				total_moves = a_moves + b_moves;
		}

		std::cout << total_moves << std::endl;
	}
	return 0;
}