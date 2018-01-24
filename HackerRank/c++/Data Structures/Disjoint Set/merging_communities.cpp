#include <cmath>
#include <cstdio>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <algorithm>

struct subset
{
	int parent;
	int rank = 0;
	int size = 1;

	subset()
	{
		parent = 0;
	}

	subset(int p)
	{
		parent = p;
	}
};

std::vector<subset> subsets;

int find(int i)
{
	if (subsets[i].parent != i)
		subsets[i].parent = find(subsets[i].parent);
	return subsets[i].parent;
}

void DoUnion(int x, int y)
{
	int x_root = find(x);
	int y_root = find(y);

	if (x_root == y_root)
		return;

	if (subsets[x_root].rank < subsets[y_root].rank)
	{
		subsets[x_root].parent = y_root;
		subsets[y_root].size += subsets[x_root].size;
	}
	else if (subsets[x_root].rank > subsets[y_root].rank)
	{
		subsets[y_root].parent = x_root;
		subsets[x_root].size += subsets[y_root].size;
	}
	else
	{
		subsets[y_root].parent = x_root;
		subsets[x_root].rank++;
		subsets[x_root].size += subsets[y_root].size;
	}
}

int CommunitiesMain()
{
	int num_people, num_queries;
	std::cin >> num_people >> num_queries;

	subsets.push_back(subset(0));
	for (int i = 1; i <= num_people; i++)
		subsets.push_back(subset(i));

	int currentQuery = 0;
	for (int i = 0; i < num_queries; i++)
	{
		char query;
		std::cin >> query;
		switch (query)
		{
			case 'M':
			{
				int first_person, second_person;
				std::cin >> first_person >> second_person;
				DoUnion(first_person, second_person);
				break;
			}
			case 'Q':
			{
				int query_person;
				std::cin >> query_person;
				int parent = find(query_person);
				std::cout << subsets[parent].size << std::endl;
				break;
			}
		}
	}
	return 0;
}