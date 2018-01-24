#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

int QueensAttack2Main()
{
	int n;
	int k;
	std::cin >> n >> k;
	int queen_row;
	int queen_col;
	std::cin >> queen_row >> queen_col;
	std::vector<int> obstacle_distance(8);

	obstacle_distance[0] = (queen_row < queen_col) ? queen_row : queen_col;
	obstacle_distance[1] = queen_row;
	obstacle_distance[2] = ((queen_row - 1) < (n - queen_col)) ? queen_row : (n + 1) - queen_col;
	obstacle_distance[3] = queen_col;
	obstacle_distance[4] = (n + 1) - queen_col;
	obstacle_distance[5] = ((n - queen_row) < (queen_col - 1)) ? (n + 1) - queen_row : queen_col;
	obstacle_distance[6] = (n + 1) - queen_row;
	obstacle_distance[7] = ((n - queen_row) < (n - queen_col)) ? (n + 1) - queen_row : (n + 1) - queen_col;

	for (int a0 = 0; a0 < k; a0++)
	{
		int obstacle_row;
		int obstacle_col;
		std::cin >> obstacle_row >> obstacle_col;

		int row_distance = queen_row - obstacle_row;
		int col_distance = queen_col - obstacle_col;
		if (row_distance == col_distance && row_distance > 0 && col_distance > 0 && row_distance < obstacle_distance[0]) //NW
			obstacle_distance[0] = row_distance;
		else if (col_distance == 0 && row_distance > 0 && row_distance < obstacle_distance[1]) //N
			obstacle_distance[1] = row_distance;
		else if (col_distance < 0 && abs(col_distance) == row_distance && row_distance < obstacle_distance[2]) //NE
			obstacle_distance[2] = row_distance;
		else if (col_distance > 0 && row_distance == 0 && col_distance < obstacle_distance[3]) //W
			obstacle_distance[3] = col_distance;
		else if (col_distance < 0 && row_distance == 0 && abs(col_distance) < obstacle_distance[4]) //E
			obstacle_distance[4] = abs(col_distance);
		else if (col_distance > 0 && row_distance < 0 && col_distance == abs(row_distance) && col_distance < obstacle_distance[5]) //SW
			obstacle_distance[5] = col_distance;
		else if (col_distance == 0 && row_distance < 0 && row_distance < obstacle_distance[6]) //S
			obstacle_distance[6] = abs(row_distance);
		else if (col_distance == row_distance && col_distance < 0 && row_distance < 0 && abs(col_distance) < obstacle_distance[7]) //SE
			obstacle_distance[7] = abs(col_distance);
	}

	int total_squares = 0;
	for (int i = 0; i < obstacle_distance.size(); i++)
		total_squares += (obstacle_distance[i] - 1);
	std::cout << total_squares << std::endl;

	return 0;
}
