#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

int EncryptionMain()
{
	std::string s;
	std::cin >> s;
	auto place_loc = s.find(' ');
	while (place_loc != std::string::npos)
	{
		s.erase(place_loc, 1);
		place_loc = s.find(' ');
	}
	float actual_length = sqrt(s.size());
	int floor_length = floor(actual_length);
	int ceil_length = ceil(actual_length);
	if (floor_length * ceil_length < s.size())
	{
		if (floor_length < ceil_length)
			floor_length = ceil_length;
		else
			ceil_length = floor_length;
	}
	std::vector<std::string> message(ceil_length);
	int loc = 0;
	int end = (ceil_length < s.size()) ? ceil_length : s.size() - loc;
	for (int i = 0; i < floor_length; i++)
	{
		std::string new_line = s.substr(loc, end);
		for (int j = 0; j < new_line.size(); j++)
			message[j] += new_line[j];
		loc += end;
		end = (loc + ceil_length < s.size()) ? ceil_length : s.size() - loc;
	}
	for (int i = 0; i < message.size(); i++)
		std::cout << message[i] << " ";

	return 0;
}