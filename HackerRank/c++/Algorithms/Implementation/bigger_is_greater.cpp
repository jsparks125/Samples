#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

int BiggerIsGreater()
{
	int strings;
	std::cin >> strings;
	for (int n = 0; n < strings; n++)
	{
		std::string word;
		std::cin >> word;
		bool swapped = false;
		for (int i = word.size() - 1; i >= 1; i--)
		{
			if (word[i] > word[i - 1])
			{
				char min_above = word[i];
				int min_above_index = i;
				for (int j = i - 1; j < word.size(); j++)
				{
					if (min_above > word[j] && word[j] > word[i - 1])
					{
						min_above = word[j];
						min_above_index = j;
					}
				}
				char temp_char = word[i - 1];
				word[i - 1] = min_above;
				word[min_above_index] = temp_char;
				sort(word.begin() + i, word.end());
				swapped = true;
				break;
			}
		}
		if (swapped)
			std::cout << word << std::endl;
		else
			std::cout << "no answer" << std::endl;
	}
	return 0;
}
