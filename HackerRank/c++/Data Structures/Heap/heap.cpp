#include <cmath>
#include <cstdio>
#include <vector>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int Heap()
{
	int num_queries;
	std::cin >> num_queries;

	std::vector<int> heap;
	for (int i = 0; i < num_queries; i++)
	{
		int query;
		std::cin >> query;
		switch (query)
		{
			case 1:
			{
				int new_element;
				std::cin >> new_element;
				heap.push_back(new_element);
				int position = heap.size() - 1;
				for (; position > 0 && new_element < heap[(position - 1) / 2]; position = (position - 1) / 2)
				{
					heap[position] = heap[(position - 1) / 2];
				}
				heap[position] = new_element;
				break;
			}
			case 2:
			{
				int delete_element;
				std::cin >> delete_element;
				int position = 0;
				for (; position < heap.size(); position++)
				{
					if (heap[position] == delete_element)
					{
						heap[position] = heap[heap.size() - 1];
						heap.pop_back();
						break;
					}
				}
					
				int parent = heap[position];
				int left_child = INT_MAX, right_child = INT_MAX;
				if (position * 2 + 1 < heap.size())
				{
					left_child = heap[position * 2 + 1];
					if (position * 2 + 2 < heap.size())
						right_child = heap[position * 2 + 2];
				}
				while (parent > left_child || parent > right_child)
				{
					int swap_child = std::min(left_child, right_child);
					int swap_position;
					if (swap_child == left_child)
						swap_position = position * 2 + 1;
					else
						swap_position = position * 2 + 2;

					heap[position] = heap[swap_position];
					heap[swap_position] = parent;
					position = swap_position;

					left_child = INT_MAX, right_child = INT_MAX;
					if (position * 2 + 1 < heap.size())
					{
						left_child = heap[position * 2 + 1];
						if (position * 2 + 2 < heap.size())
							right_child = heap[position * 2 + 2];
					}
				}

				break;
			}
			case 3:
			{
				std::cout << heap.front() << std::endl;
				break;
			}
		}
	}
		return 0;
}