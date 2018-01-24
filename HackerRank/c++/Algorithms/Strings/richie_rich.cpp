#include <string>
#include <cstdio>
#include <iostream>
#include <algorithm>

std::string RichieRich(std::string s, int n, int k)
{
	if (k > n)
		return std::string(n, '9');

	std::string first_half = s.substr(0, n / 2);
	std::string second_half = (s.substr((n / 2) + (n % 2), s.size() / 2));
	if (k == 0)
	{
		if (first_half == second_half)
			return s;
		return "-1";
	}
	if (s.size() == 1)
		return "9";

	std::reverse(second_half.begin(), second_half.end());
	for (int p = 0; p < 2; p++)
	{
		for (int i = 0; i < n / 2; i++)
		{
			char first_int = first_half[i];
			char second_int = second_half[i];
			if (p == 1 && k == 1)
			{
				if (first_int == 'a' || second_int == 'a')
				{
					if (first_half[i] != '9' && second_half[i] != '9')
						k--;
					first_half[i] = '9';
					second_half[i] = '9';
				}
			}
			else if (k >= 2)
			{
				if (p == 0)
				{
					if (first_int != second_int)
					{
						if (first_int > second_int)
						{
							second_half[i] = 'a';
							k--;
						}
						else if (first_int < second_int)
						{
							first_half[i] = 'a';
							k--;
						}
					}
				}
				else
				{
					if (first_int != '9')
					{
						if (first_int != 'a')
							k--;
						first_half[i] = '9';
					}
					if (second_int != '9')
					{
						if (second_int != 'a')
							k--;
						second_half[i] = '9';
					}
				}
			}
			else if (k > 0)
			{
				if (first_int == 'a')
					first_half[i] = second_half[i];
				else if (second_int == 'a')
					second_half[i] = first_half[i];
				else if (first_int > second_int)
				{
					second_half[i] = first_int;
					k--;
				}
				else if (first_int < second_int)
				{
					first_half[i] = second_int;
					k--;
				}
			}
		}
	}

	for (int i = 0; i < first_half.size(); i++)
	{
		if (first_half[i] == 'a')
			first_half[i] = second_half[i];
		if (second_half[i] == 'a')
			second_half[i] = first_half[i];
	}

	if (k == 1)
	{
		if (n % 2 == 1)
		{
			if (first_half == second_half)
			{
				std::reverse(second_half.begin(), second_half.end());
				return first_half + '9' + second_half;
			}
			else
				return "-1";
		}
	}

	if (first_half == second_half)
	{
		std::reverse(second_half.begin(), second_half.end());
		if (n % 2 == 1)
			return first_half + s[n / 2] + second_half;
		else
			return first_half + second_half;
	}
	return "-1";
}

int main()
{
	int n;
	std::cin >> n;
	int k;
	std::cin >> k;
	std::string s;
	std::cin >> s;
	std::string result = RichieRich(s, n, k);
	std::cout << result << std::endl;
	return 0;
}