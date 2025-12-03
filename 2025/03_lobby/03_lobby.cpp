#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::string>& data);
int part_one(std::vector<std::string>& data);
std::uint64_t part_two(std::vector<std::string>& data);

int main()
{
	// 174.60µs : input input day03
	// 15.200µs : part1
	// 36.300µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day03");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	std::vector<std::string> data;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<std::string>& data)
{
	std::ifstream input(input_path);

	std::string line;
	while (std::getline(input, line))
		data.emplace_back(std::move(line));
}

int part_one(std::vector<std::string>& data)
{
	int result{ 0 };

	for (auto& row : data)
	{
		std::int8_t digit_pos{ -1 };
		for (std::uint8_t dig{ 2 }; dig > 0; --dig)
		{
			std::uint8_t digit{ 0 };
			for (std::int8_t i{ ++digit_pos }; i <= row.size() - dig; ++i)
			{
				if (row[i] > digit)
				{
					digit = row[i] - '0';
					digit_pos = i;
				}
			}
			result += digit * std::pow(10, dig - 1);
		}
	}

	return result;
}

std::uint64_t part_two(std::vector<std::string>& data)
{
	std::uint64_t result{ 0 };

	for (auto& row : data)
	{
		std::int8_t digit_pos{ -1 };
		for (std::uint8_t dig{ 12 }; dig > 0; --dig)
		{
			std::uint8_t digit{ 0 };
			for (std::int8_t i{ ++digit_pos }; i <= row.size() - dig; ++i)
			{
				if (row[i] > digit)
				{
					digit = row[i] - '0';
					digit_pos = i;
				}
			}
			result += digit * std::pow(10, dig - 1);
		}
	}

	return result;
}