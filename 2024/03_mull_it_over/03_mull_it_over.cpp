#include "../../other/include_everything.h"
import std;

void read_data(std::string& data);
int part_one(std::string& data);
int part_two(std::string& data);

int main()
{
	std::string data;

	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 4.6812ms  : data in + parsing
	// 0.7123ms  : part1
	// 0.9263ms  : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration();

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration();

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration();

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::string& data)
{
	std::ifstream input(__INPUT_PATH);
	std::string row;
	while (std::getline(input, row))
	{
		data += row;
	}
}

int part_one(std::string& data)
{
	int result = 0;

	std::regex reg_word{ R"(mul\(\d+,\d+\))" };

	std::sregex_iterator currentMatch(data.begin(), data.end(), reg_word);
	std::sregex_iterator lastMatch;
	while (currentMatch != lastMatch)
	{
		std::smatch temp = *currentMatch;
		int comma_pos = temp.str().find_first_of(',');
		int first_number = std::stoi(temp.str().substr(4, comma_pos - 4));
		int second_number = std::stoi(temp.str().substr(comma_pos + 1, temp.str().size() - 1));
		result += first_number * second_number;
		++currentMatch;
	}

	return result;
}

int part_two(std::string& data)
{
	int result = 0;
	bool is_enabled = true;

	std::regex reg_word{ R"(mul\(\d+,\d+\)|do\(\)|don't\(\))" };

	std::sregex_iterator currentMatch(data.begin(), data.end(), reg_word);
	std::sregex_iterator lastMatch;
	while (currentMatch != lastMatch)
	{
		std::smatch temp = *currentMatch;
		if (is_enabled && temp.str()[0] == 'm')
		{
			int comma_pos = temp.str().find_first_of(',');
			int first_number = std::stoi(temp.str().substr(4, comma_pos - 4));
			int second_number = std::stoi(temp.str().substr(comma_pos + 1, temp.str().size() - 1));
			result += first_number * second_number;
		}
		else
		{
			if (temp.str()[2] == 'n') is_enabled = false;
			if (temp.str()[2] == '(') is_enabled = true;
		}

		++currentMatch;
	}

	return result;
	return result;
}