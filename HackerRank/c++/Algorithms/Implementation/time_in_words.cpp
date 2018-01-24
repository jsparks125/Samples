#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

std::string IntToWord(int i)
{
	switch (i)
	{
	case 10: return "ten";
	case 11: return "eleven";
	case 12: return "twelve";
	case 13: return "thirteen";
	case 15: return "fifteen";
	}

	std::string int_string = std::to_string(i);
	std::string first_digit = "";
	int first_index = 1;
	if (int_string.size() == 1)
		first_index = 0;
	switch (int_string[first_index])
	{
	case '1': first_digit = "one"; break;
	case '2': first_digit = "two"; break;
	case '3': first_digit = "three"; break;
	case '4': first_digit = "four"; break;
	case '5': first_digit = "five"; break;
	case '6': first_digit = "six"; break;
	case '7': first_digit = "seven"; break;
	case '8': first_digit = "eight"; break;
	case '9': first_digit = "nine"; break;
	}
	if (int_string.size() == 1)
		return first_digit;
	switch (int_string[0])
	{
	case '1': return first_digit + "teen";
	case '2': return "twenty " + first_digit;
	case '3': return "thirty " + first_digit;
	case '4': return "forty " + first_digit;
	case '5': return "fifty" + first_digit;
	}
	return "";
}

int TimeInWords()
{
	int hour, minute;
	std::cin >> hour >> minute;
	std::string minute_string = "";
	std::string gap_string = "";
	if (minute == 0)
	{
		std::cout << IntToWord(hour) << " o' clock";
		return 0;
	}

	if (minute == 15 || minute == 45)
		minute_string = "quarter";
	else if (minute == 30)
		minute_string = "half";
	else if (minute == 1 || minute == 59)
		minute_string = "one minute";
	else
	{
		minute_string = "minutes";
		if (minute < 30)
			minute_string = IntToWord(minute) + " " + minute_string;
		else if (minute > 30)
			minute_string = IntToWord(60 - minute) + " " + minute_string;
	}
	if (minute <= 30)
		gap_string = "past";
	else
	{
		gap_string = "to";
		hour++;
	}

	std::cout << minute_string << " " << gap_string << " " << IntToWord(hour);

	return 0;
}
