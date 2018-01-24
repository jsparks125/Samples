#include <cmath>
#include <cstdio>
#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>

struct pump
{
	int petrol, distance;
	pump(int p, int d)
	{
		petrol = p;
		distance = d;
	}
};

int TruckTour()
{
	int n;
	std::cin >> n;
	std::queue<pump> pumps;
	for (int i = 0; i < n; i++)
	{
		int petrol, distance;
		std::cin >> petrol >> distance;
		pumps.push(pump(petrol, distance));
	}

	int start_pump = 0;
	int pumps_passed = 0;
	int current_petrol = 0;
	while (pumps_passed < n)
	{
		pump current_pump = pumps.front();
		current_petrol += current_pump.petrol;
		pumps.pop();

		if (current_petrol >= current_pump.distance)
		{
			pumps_passed++;
			current_petrol -= current_pump.distance;
		}
		else
		{
			start_pump += pumps_passed + 1;
			pumps_passed = 0;
			current_petrol = 0;
		}
		pumps.push(current_pump);
	}
	std::cout << start_pump << std::endl;
	return 0;
}