#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int AlmostSorted()
{
	int n;
	std::cin >> n;
	if (n == 2)
	{
		std::cout << "yes" << std::endl;
		std::cout << "swap 1 2" << std::endl;
		return 0;
	}
	int unsorted_start = -1;
	int unsorted_end = -1;
	int last_d = -1;
	int unsorted_direction = 0;
	std::vector<int> a(n + 1);
	a.push_back(0);
	for (int i = 1; i <= n; i++)
	{
		int d;
		std::cin >> d;
		a[i] = d;
		if (d < last_d && unsorted_start == -1)
			unsorted_start = i - 1;
		else if (d < last_d)
		{
			if (unsorted_start > 0 && unsorted_end == -1)
			{
				if (unsorted_direction == 1)
					unsorted_end = i;
				else
					unsorted_direction = -1;
			}
			else if (unsorted_start > 0 && unsorted_end > 0)
			{
				std::cout << "no" << std::endl;
				return 0;
			}
		}
		else if (d > last_d && unsorted_start > 0 && unsorted_end == -1)
		{
			if (unsorted_direction == -1)
				unsorted_end = i - 1;
			else
				unsorted_direction = 1;
		}
		last_d = d;
	}

	if (unsorted_end == -1)
		unsorted_end = n;

	if (a[unsorted_start - 1] + 1 == a[unsorted_start + 1] && a[unsorted_start] == a[unsorted_start + 1] + 1)
	{
		std::cout << "yes" << std::endl;
		std::cout << "swap " << unsorted_start << " " << unsorted_start + 1 << std::endl;
		return 0;
	}

	if (unsorted_start == unsorted_end - 1)
	{
		std::cout << "yes" << std::endl;
		std::cout << "swap " << unsorted_start << " " << unsorted_end << std::endl;
		return 0;
	}

	if (unsorted_direction == 1)
	{
		if (a[unsorted_start - 1] < a[unsorted_end] && a[unsorted_end] < a[unsorted_start + 1])
		{
			if (a[unsorted_end - 1] < a[unsorted_start] && a[unsorted_start] < a[unsorted_end + 1])
			{
				std::cout << "yes" << std::endl;
				std::cout << "swap " << unsorted_start << " " << unsorted_end << std::endl;
				return 0;
			}
		}
	}
	else if (unsorted_direction == -1)
	{
		std::cout << "yes" << std::endl;
		std::cout << "reverse " << unsorted_start << " " << unsorted_end << std::endl;
		return 0;
	}

	std::cout << "no" << std::endl;

	return 0;
}
