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

int OrganizingContainersBalls()
{
	int q;
	std::cin >> q;
	for (int a0 = 0; a0 < q; a0++)
	{
		int n;
		std::cin >> n;
		std::vector<long> types(n);
		std::vector<long> containers(n);
		for (int M_i = 0; M_i < n; M_i++)
		{
			for (int M_j = 0; M_j < n; M_j++)
			{
				int num_type;
				std::cin >> num_type;
				types[M_j] += num_type;
				containers[M_i] += num_type;
			}
		}
		sort(types.begin(), types.end());
		sort(containers.begin(), containers.end());
		bool mismatch = false;
		for (int i = 0; i < types.size(); i++)
		{
			if (types[i] != containers[i])
				mismatch = true;
		}
		if (mismatch)
			std::cout << "Impossible" << std::endl;
		else
			std::cout << "Possible" << std::endl;
	}
	return 0;
}
