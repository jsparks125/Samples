#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>

int MatrixLayerRotation()
{
	int m, n, r;
	std::cin >> m >> n >> r;
	std::vector<std::vector<int>> grid(m, std::vector<int>(n));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
			std::cin >> grid[i][j];
	}

	int layers = (n / 2) + (n % 2);
	if (m < n)
		layers = (m / 2) + (m % 2);

	std::vector<std::vector<int>> new_grid(m, std::vector<int>(n));
	for (int i = 0; i < layers; i++)
	{
		int num_horizontal = n - (2 * i);
		int num_vertical = (m - (2 * i)) - 2; //-2 to remove the corners
		int actual_rotations = r % ((2 * num_horizontal) + (2 * num_vertical));

		//move top row including corners
		for (int tr = i; tr < i + num_horizontal; tr++)
		{
			int remaining_rotations = actual_rotations;
			int moves_left = tr - i;
			if (remaining_rotations <= moves_left)
			{
				new_grid[i][tr - remaining_rotations] = grid[i][tr];
				continue;
			}
			remaining_rotations -= moves_left;
			int moves_down = num_vertical + 1;
			if (remaining_rotations <= moves_down)
			{
				new_grid[i + remaining_rotations][i] = grid[i][tr];
				continue;
			}
			remaining_rotations -= moves_down;
			int moves_right = num_horizontal - 1;
			if (remaining_rotations <= moves_right)
			{
				new_grid[i + num_vertical + 1][i + remaining_rotations] = grid[i][tr];
				continue;
			}
			remaining_rotations -= moves_right;
			int moves_up = num_vertical + 1;
			if (remaining_rotations <= moves_up)
			{
				new_grid[(i + num_vertical + 1) - remaining_rotations][i + num_horizontal - 1] = grid[i][tr];
				continue;
			}
			remaining_rotations -= moves_up;
			moves_left = num_horizontal - 1;
			if (remaining_rotations <= moves_left)
				new_grid[i][(i + num_horizontal - 1) - remaining_rotations] = grid[i][tr];
		}
		//move left column, not including corners
		for (int lc = i + 1; lc < (i + num_vertical + 1); lc++)
		{
			int remaining_rotations = actual_rotations;
			int moves_down = (i + num_vertical + 1) - lc;
			if (remaining_rotations <= moves_down)
			{
				new_grid[lc + remaining_rotations][i] = grid[lc][i];
				continue;
			}
			remaining_rotations -= moves_down;
			int moves_right = num_horizontal - 1;
			if (remaining_rotations <= moves_right)
			{
				new_grid[i + num_vertical + 1][i + remaining_rotations] = grid[lc][i];
				continue;
			}
			remaining_rotations -= moves_right;
			int moves_up = num_vertical + 1;
			if (remaining_rotations <= moves_up)
			{
				new_grid[(i + num_vertical + 1) - remaining_rotations][i + num_horizontal - 1] = grid[lc][i];
				continue;
			}
			remaining_rotations -= moves_up;
			int moves_left = num_horizontal - 1;
			if (remaining_rotations <= moves_left)
			{
				new_grid[i][(i + num_horizontal - 1) - remaining_rotations] = grid[lc][i];
				continue;
			}
			remaining_rotations -= moves_left;
			moves_down = num_vertical + 1;
			if (remaining_rotations <= moves_down)
				new_grid[i + remaining_rotations][i] = grid[lc][i];
		}
		//move bottom row including corners
		for (int br = i; br < i + num_horizontal; br++)
		{
			int remaining_rotations = actual_rotations;
			int moves_right = (i + num_horizontal - 1) - br;
			if (remaining_rotations <= moves_right)
			{
				new_grid[i + num_vertical + 1][br + remaining_rotations] = grid[i + num_vertical + 1][br];
				continue;
			}
			remaining_rotations -= moves_right;
			int moves_up = num_vertical + 1;
			if (remaining_rotations <= moves_up)
			{
				new_grid[(i + num_vertical + 1) - remaining_rotations][i + num_horizontal - 1] = grid[i + num_vertical + 1][br];
				continue;
			}
			remaining_rotations -= moves_up;
			int moves_left = num_horizontal - 1;
			if (remaining_rotations <= moves_left)
			{
				new_grid[i][(i + num_horizontal - 1) - remaining_rotations] = grid[i + num_vertical + 1][br];
				continue;
			}
			remaining_rotations -= moves_left;
			int moves_down = num_vertical + 1;
			if (remaining_rotations <= moves_down)
			{
				new_grid[i + remaining_rotations][i] = grid[i + num_vertical + 1][br];
				continue;
			}
			remaining_rotations -= moves_down;
			moves_right = num_horizontal - 1;
			if (remaining_rotations <= moves_right)
				new_grid[i + num_vertical + 1][i + remaining_rotations] = grid[i + num_vertical + 1][br];
		}
		//move right column not including corners
		for (int rc = i + 1; rc < (i + num_vertical + 1); rc++)
		{
			int remaining_rotations = actual_rotations;
			int moves_up = rc - i;
			if (remaining_rotations <= moves_up)
			{
				new_grid[rc - remaining_rotations][i + num_horizontal - 1] = grid[rc][i + num_horizontal - 1];
				continue;
			}
			remaining_rotations -= moves_up;
			int moves_left = num_horizontal - 1;
			if (remaining_rotations <= moves_left)
			{
				new_grid[i][(i + num_horizontal - 1) - remaining_rotations] = grid[rc][i + num_horizontal - 1];
				continue;
			}
			remaining_rotations -= moves_left;
			int moves_down = num_vertical + 1;
			if (remaining_rotations <= moves_down)
			{
				new_grid[i + remaining_rotations][i] = grid[rc][i + num_horizontal - 1];
				continue;
			}
			remaining_rotations -= moves_down;
			int moves_right = num_horizontal - 1;
			if (remaining_rotations <= moves_right)
			{
				new_grid[i + num_vertical + 1][i + remaining_rotations] = grid[rc][i + num_horizontal - 1];
				continue;
			}
			remaining_rotations -= moves_right;
			moves_up = num_vertical + 1;
			if (remaining_rotations <= moves_up)
				new_grid[(i + num_vertical + 1) - remaining_rotations][i + num_horizontal - 1] = grid[rc][i + num_horizontal - 1];
		}
	}

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (j < n - 1)
				std::cout << new_grid[i][j] << " ";
			else
				std::cout << new_grid[i][j];
		}
		std::cout << std::endl;
	}

	return 0;
}
