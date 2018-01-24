#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

int BomberMan()
{
	int r, c, n;
	std::cin >> r >> c >> n;
	std::vector<std::string> grid(r);
	if (n % 2 == 0)
	{
		for (int i = 0; i < r; i++)
		{
			for (int j = 0; j < c; j++)
				std::cout << "O";
			std::cout << std::endl;
		}
		return 0;
	}

	for (int i = 0; i < r; i++)
	{
		std::cin >> grid[i];
		if (n == 1)
			std::cout << grid[i] << std::endl;
	}

	if (n == 1)
		return 0;

	int actual_turns = 1;
	if ((n - 3) % 4 == 2)
		actual_turns = 2;

	for (int at = 0; at < actual_turns; at++)
	{
		std::vector<std::string> temp_grid(r);
		for (int i = 0; i < r; i++)
		{
			std::string opposite_string = "";
			for (int j = 0; j < c; j++)
			{
				bool up_clear = true;
				if ((i - 1) >= 0 && grid[i - 1][j] == 'O')
					up_clear = false;
				bool down_clear = true;
				if ((i + 1) < r && grid[i + 1][j] == 'O')
					down_clear = false;
				bool left_clear = true;
				if ((j - 1) >= 0 && grid[i][j - 1] == 'O')
					left_clear = false;
				bool right_clear = true;
				if ((j + 1) < c && grid[i][j + 1] == 'O')
					right_clear = false;

				if (up_clear && down_clear && left_clear && right_clear && grid[i][j] == '.')
					opposite_string += 'O';
				else
					opposite_string += '.';
			}
			if (at == actual_turns - 1)
				std::cout << opposite_string << std::endl;
			else
				temp_grid[i] = opposite_string;
		}
		grid = temp_grid;
	}

	return 0;
}