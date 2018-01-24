#include <cmath>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <string>

std::string AddStrings(std::string a, std::string b)
{
	if (a.size() > b.size())
		b = std::string(a.size() - b.size(), '0') + b;
	else
		a = std::string(b.size() - a.size(), '0') + a;

	std::string result = "";
	char overflow = '0';
	for (int i = a.size() - 1; i >= 0; i--)
	{
		std::string add_result = std::to_string((a[i] - 48) + (b[i] - 48) + (overflow - 48));
		if (add_result.size() > 1)
		{
			overflow = add_result[0];
			result = add_result[1] + result;
		}
		else
		{
			overflow = '0';
			result = add_result[0] + result;
		}
	}
	if (overflow != '0')
		result = overflow + result;
	return result;
}

std::string MultiplyStrings(std::string a, std::string b)
{
	std::string result = "0";
	for (int i = a.size() - 1; i >= 0; i--)
	{
		int a_extra_zeroes = a.size() - i - 1;
		std::string temp_result = "0";
		for (int j = b.size() - 1; j >= 0; j--)
		{
			int b_extra_zeroes = b.size() - j - 1;
			std::string multiply_result = std::to_string((a[i] - 48) * (b[j] - 48)) + std::string(b_extra_zeroes, '0');
			temp_result = AddStrings(temp_result, multiply_result);
		}
		result = AddStrings(result, (temp_result + std::string(a_extra_zeroes, '0')));
	}
	return result;
}

int ExtraLongFactorialMain()
{
	int n;
	std::cin >> n;
	std::string result = "1";
	for (int i = 2; i <= n; i++)
		result = MultiplyStrings(result, std::to_string(i));
	std::cout << result << std::endl;
	return 0;
}