#include <chrono>
#include <cmath>
#include <cstdio>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <string>

int EvaluateDays(std::vector<std::vector<int>> delete_ranges)
{
	int num_days = 0;
	while (delete_ranges.size() > 0)
	{
		num_days++;
		int previous_element = -1;
		for (int i = 0; i < delete_ranges.size(); i++)
		{
			std::vector<int> *current_range = &delete_ranges[i];
			int first_element = (*current_range)[0];
			int next_previous = (*current_range)[current_range->size() - 1];
			if (first_element > previous_element)
			{
				current_range->erase(current_range->begin());
			}
			previous_element = next_previous;

			if (current_range->empty())
			{
				delete_ranges.erase(delete_ranges.begin() + i);
				i--;
			}
		}
	}
	return num_days;
}

int PoisonousPlants() 
{
	int n;
	std::cin >> n;
	std::vector<int> p(n);
	for (int p_i = 0; p_i < n; p_i++)
		std::cin >> p[p_i];

	int max_deletes = 0;
	std::vector<int> delete_range;
	std::vector<std::vector<int>> delete_ranges;
	int min_level = p[0];
	bool in_delete = false;
	int max_range_size = 0;
	int current_range_size = 0;
	int range_min = -1;
	bool needs_calc = false;
	for (int i = 1; i < p.size(); i++)
	{
		if (min_level >= p[i])
		{
			min_level = p[i];
			in_delete = false;
			int current_deletes;

			if (delete_ranges.size() > 0 && delete_range.size() > 0)
				delete_ranges.push_back(delete_range);

			if (current_range_size > max_range_size)
				max_range_size = current_range_size;

			if (needs_calc)
				current_deletes = EvaluateDays(delete_ranges);
			else
				current_deletes = max_range_size;

			needs_calc = false;
			if (current_deletes > max_deletes)
				max_deletes = current_deletes;

			delete_range.clear();
			delete_ranges.clear();

			current_range_size = 0;
		}
		else
		{
			if (in_delete)
			{
				if (p[i - 1] < p[i])
				{
					if (range_min == -1)
						range_min = p[i - 1];
					else if (range_min > p[i])
						range_min = p[i];

					delete_ranges.push_back(delete_range);
					delete_range.clear();

					if (current_range_size > max_range_size)
						max_range_size = current_range_size;
					current_range_size = 1;
				}
				else
					current_range_size++;

				if (range_min != -1)
				{
					if (range_min >= p[i])
						needs_calc = true;
				}
			}
			else
			{
				in_delete = true;
				max_range_size = 0;
				range_min = -1;
				current_range_size = 1;
				delete_range.clear();
				delete_ranges.clear();
			}
			
			delete_range.push_back(p[i]);
		}
	}

	if (delete_ranges.size() > 0 && delete_range.size() > 0)
		delete_ranges.push_back(delete_range);

	int current_deletes;
	if (current_range_size > max_range_size)
		max_range_size = current_range_size;

	if (needs_calc)
		current_deletes = EvaluateDays(delete_ranges);
	else
		current_deletes = max_range_size;

	if (current_deletes > max_deletes)
		max_deletes = current_deletes;

	std::cout << max_deletes << std::endl;
	return 0;
}