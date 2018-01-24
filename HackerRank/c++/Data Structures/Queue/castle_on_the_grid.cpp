#include <cmath>
#include <cstdio>
#include <map>
#include <vector>
#include <set>
#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int MinimumMoves(std::vector<std::string> grid, int start_x, int start_y, int goal_x, int goal_y)
{
	if (start_x == goal_x && start_y == goal_y)
		return 0;

	std::vector<std::pair<int, int>> queue;
	std::set<std::pair<int, int>> encountered;
	std::map<std::pair<int, int>, int> steps;

	std::pair<int, int> current_node = std::pair<int, int>(start_x, start_y);
	queue.push_back(current_node);
	steps[current_node] = 0;
	while (queue.size() > 0)
	{
		std::pair<int, int> current_node = queue.front();
		queue.erase(queue.begin());
		encountered.insert(current_node);

		int left_x = current_node.first - 1;
		int right_x = current_node.first + 1;
		int up_y = current_node.second - 1;
		int down_y = current_node.second + 1;

		while (left_x >= 0)
		{
			std::pair<int, int> left_pair = std::pair<int, int>(left_x, current_node.second);
			if (encountered.find(left_pair) == encountered.end())
			{
				steps[left_pair] = steps[current_node] + 1;
				if (left_x == goal_x && current_node.second == goal_y)
					return steps[left_pair];
				if (grid[current_node.second][left_x] == 'X')
					break;
				queue.push_back(left_pair);
				encountered.insert(left_pair);
			}
			left_x--;
		}
		while (right_x <= grid.size() - 1)
		{
			std::pair<int, int> right_pair = std::pair<int, int>(right_x, current_node.second);
			if (encountered.find(right_pair) == encountered.end())
			{
				steps[right_pair] = steps[current_node] + 1;
				if (right_x == goal_x && current_node.second == goal_y)
					return steps[right_pair];
				if (grid[current_node.second][right_x] == 'X')
					break;
				queue.push_back(right_pair);
				encountered.insert(right_pair);
			}
			right_x++;
		}
		while (up_y >= 0)
		{
			std::pair<int, int> up_pair = std::pair<int, int>(current_node.first, up_y);
			if (encountered.find(up_pair) == encountered.end())
			{
				steps[up_pair] = steps[current_node] + 1;
				if (current_node.first == goal_x && up_y == goal_y)
					return steps[up_pair];
				if (grid[up_y][current_node.first] == 'X')
					break;
				queue.push_back(up_pair);
				encountered.insert(up_pair);
			}
			up_y--;
		}
		while (down_y <= grid.size() - 1)
		{
			std::pair<int, int> down_pair = std::pair<int, int>(current_node.first, down_y);
			if (encountered.find(down_pair) == encountered.end())
			{
				steps[down_pair] = steps[current_node] + 1;
				if (current_node.first == goal_x && down_y == goal_y)
					return steps[down_pair];
				if (grid[down_y][current_node.first] == 'X')
					break;
				queue.push_back(down_pair);
				encountered.insert(down_pair);
			}
			down_y++;
		}
	}
	return 0;
}

int CastleOnTheGrid()
{
	int n;
	std::cin >> n;
	std::vector<std::string> grid(n);
	for (int grid_i = 0; grid_i < n; grid_i++) 
	{
		std::string new_string;
		for (int grid_j = 0; grid_j < n; grid_j++)
		{
			char c;
			std::cin >> c;
			new_string += c;
		}
		grid[grid_i] = new_string;
	}
	int start_x;
	int start_y;
	int goal_x;
	int goal_y;
	std::cin >> start_x >> start_y >> goal_x >> goal_y;
	int result = MinimumMoves(grid, start_y, start_x, goal_y, goal_x);
	std::cout << result;
	return 0;
}