#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

struct cross
{
	int row, col, size;
	cross(int r, int c, int s)
	{
		row = r;
		col = c;
		size = s;
	}
};

int CrossesProduct(cross a, cross b)
{
	int a_min_col = a.col - a.size;
	int a_max_col = a.col + a.size;
	int a_min_row = a.row - a.size;
	int a_max_row = a.row + a.size;
	int b_min_col = b.col - b.size;
	int b_max_col = b.col + b.size;
	int b_min_row = b.row - b.size;
	int b_max_row = b.row + b.size;

	int a_size_min = a.size;
	int a_size_max = a.size;
	int b_size_min = b.size;
	int b_size_max = b.size;
	if (a.row == b.row && b_min_col <= a_max_col)
	{
		int overlap_size = a_max_col - b_min_col;
		int a_new_max_col = a_max_col - overlap_size;
		int b_new_min_col = b_min_col + overlap_size;
		a_size_min = (a_new_max_col - 1) - a.col;
		a_size_max = a_new_max_col - a.col;
		b_size_min = b.col - (b_new_min_col + 1);
		b_size_max = b.col - b_new_min_col;
	}
	if (a.col == b.col && b_min_row <= a_max_row)
	{
		int overlap_size = a_max_row - b_min_row;
		int a_new_max_row = a_max_row - overlap_size;
		int b_new_min_row = b_min_row + overlap_size;
		a_size_min = (a_new_max_row - 1) - a.row;
		a_size_max = a_new_max_row - a.row;
		b_size_min = b.row - (b_new_min_row + 1);
		b_size_max = b.row - b_new_min_row;
	}

	if (b.row > a.row && b.col > a.col)
	{
		//b is SE of a
		if (b_min_row <= a.row && b.col <= a_max_col)
		{
			//b's N vert overlaps a's E horiz
			int a_new_max_col = b.col;
			int b_new_min_row = a.row;
			a_size_min = (a_new_max_col - 1) - a.col;
			b_size_min = b.row - (b_new_min_row + 1);
		}
		if (a_max_row >= b.row && b_min_col <= a.col)
		{
			//a's S vert overlaps b's W horiz
			int a_new_max_row = b.row;
			int b_new_min_col = a.col;
			int a_size_min_temp = (a_new_max_row - 1) - a.row;
			int b_size_min_temp = b.col - (b_new_min_col + 1);
			if (a_size_min_temp < a_size_min)
				a_size_min = a_size_min_temp;
			if (b_size_min_temp < b_size_min)
				b_size_min = b_size_min_temp;
		}
	}
	if (b.row > a.row && b.col < a.col)
	{
		//b is SW of a
		if (b_max_col >= a.col && a_max_row >= b.row)
		{
			//a's S vert overlaps b's E horiz
			int a_new_max_row = b.row;
			int b_new_max_col = a.col;
			a_size_min = (a_new_max_row - 1) - a.row;
			b_size_min = (b_new_max_col - 1) - b.col;
		}
		if (a_min_col <= b.col && b_min_row <= a.row)
		{
			//b's N vert overlaps a's W horiz
			int a_new_min_col = b.col;
			int b_new_min_row = a.row;
			int a_size_min_temp = a.col - (a_new_min_col + 1);
			int b_size_min_temp = b.row - (b_new_min_row + 1);
			if (a_size_min_temp < a_size_min)
				a_size_min = a_size_min_temp;
			if (b_size_min_temp < b_size_min)
				b_size_min = b_size_min_temp;
		}
	}

	a_size_min = (a_size_min > 0) ? a_size_min : 0;
	a_size_max = (a_size_max > 0) ? a_size_max : 0;
	b_size_min = (b_size_min > 0) ? b_size_min : 0;
	b_size_max = (b_size_max > 0) ? b_size_max : 0;
	int a_min_product = ((a_size_min * 4) + 1) * ((b_size_max * 4) + 1);
	int b_min_product = ((b_size_min * 4) + 1) * ((a_size_max * 4) + 1);
	if (a_size_max - a_size_min > 1 && (b_size_max - b_size_min == a_size_max - a_size_min))
	{
		for (int i = 1; i < a_size_max - a_size_min; i++)
		{
			int new_a_min_product = (((a_size_min + i) * 4) + 1) * (((b_size_max - i) * 4) + 1);
			if (new_a_min_product > a_min_product)
				a_min_product = new_a_min_product;
			int new_b_min_product = (((b_size_min + i) * 4) + 1) * (((a_size_max - i) * 4) + 1);
			if (new_b_min_product > b_min_product)
				b_min_product = new_b_min_product;
		}
	}
	if (a_min_product > b_min_product)
		return a_min_product;
	else
		return b_min_product;
}

int EmasSupercomputer()
{
	int r, c;
	std::cin >> r >> c;

	std::vector<std::string> grid(r);
	for (int i = 0; i < r; i++)
		std::cin >> grid[i];

	std::vector<cross> crosses;
	for (int i = 1; i < r - 1; i++)
	{
		for (int j = 1; j < c - 1; j++)
		{
			if (grid[i][j] == 'G')
			{
				int max_squares = j + 1;
				if (i + 1 < max_squares)
					max_squares = i + 1;
				if (r - i < max_squares)
					max_squares = r - i;
				if (c - j < max_squares)
					max_squares = c - j;

				int num_valid_squares = 0;
				for (int ms = 1; ms < max_squares; ms++)
				{
					bool above_good = (grid[i][j - ms] == 'G') ? true : false;
					bool below_good = (grid[i][j + ms] == 'G') ? true : false;
					bool left_good = (grid[i - ms][j] == 'G') ? true : false;
					bool right_good = (grid[i + ms][j] == 'G') ? true : false;

					if (above_good && below_good && left_good && right_good)
						num_valid_squares++;
					else
						break;
				}
				crosses.push_back(cross(i, j, num_valid_squares));
			}
		}
	}

	int max_product = 0;
	for (int i = 0; i < crosses.size(); i++)
	{
		for (int j = i + 1; j < crosses.size(); j++)
		{
			int cross_product = CrossesProduct(crosses[i], crosses[j]);
			if (cross_product > max_product)
				max_product = cross_product;
		}
	}
	std::cout << max_product << std::endl;

	return 0;
}