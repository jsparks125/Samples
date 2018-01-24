#include <cstdio>
#include <map>
#include <queue>
#include <istream>
#include <iostream>
#include <string>

struct node
{
	int value, depth;

	node(int v)
	{
		value = v;
		depth = 0;
	}

	node(int v, int d)
	{
		value = v;
		depth = d;
	}
};

int DownToZeroII()
{
	std::map<int, int> min_moves;
	int q;
	std::cin >> q;
	for (int a0 = 0; a0 < q; a0++)
	{
		int n;
		std::cin >> n;

		int num_moves = 0;
		std::queue<node> nums;
		nums.push(node(n));
		while (nums.size() > 0)
		{
			node current_num = nums.front();

			if (current_num.value <= 4)
			{
				if (current_num.value == 4)
					std::cout << current_num.depth + 3 << std::endl;
				else
					std::cout << current_num.depth + current_num.value << std::endl;
				break;
			}

			nums.pop();
			nums.push(node(current_num.value - 1, current_num.depth + 1));
			int sqrt_n = sqrt(current_num.value);
			for (int i = sqrt_n; i >= 2; i--)
				if (current_num.value % i == 0)
					nums.push(node(current_num.value / i, current_num.depth + 1));
		}
	}
	return 0;
}