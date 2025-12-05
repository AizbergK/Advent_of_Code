#include "../../other/include_everything.h"
import std;

typedef std::pair<std::vector<std::int64_t>, std::vector<std::int64_t>> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
int part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 901.80µs : input input day05
	// 93.000µs : part1
	// 50.600µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day05");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	std::uint64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	data_type data;

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

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);

	std::int64_t num1, num2;
	char ch;
	std::string temp;
	while (std::getline(input, temp) && temp.size() != 0)
	{
		std::stringstream ss{ temp };
		ss >> num1 >> ch >> num2;
		data.first.emplace_back(num1);
		data.first.emplace_back(num2);
		num1 >> ch;
	}

	while (input >> num1)
	{
		data.second.emplace_back(num1);
	}
}

int part_one(data_type& data)
{
	int result{ 0 };

	for (auto id : data.second)
	{
		for (int i{ 0 }; i < data.first.size(); i += 2)
		{
			if (id >= data.first[i] && id <= data.first[i + 1])
			{
				++result;
				break;
			}
		}
	}

	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 0 };

	for (int i{ 0 }; i < data.first.size(); i += 2)
	{
		if (data.first[i] == -1) continue;

		for (int j{ 0 }; j < data.first.size(); j += 2)
		{
			if (j == i || data.first[j] == -1) continue;

			bool merged_intervals{ false };
			if (data.first[j]	  >= data.first[i] && data.first[j]		<= data.first[i + 1] ||
				data.first[j + 1] >= data.first[i] && data.first[j + 1] <= data.first[i + 1])
			{
				if (data.first[j + 1] > data.first[i + 1])
				{
					data.first[i + 1] = data.first[j + 1];
					merged_intervals = true;
				}
				if (data.first[j] < data.first[i])
				{
					data.first[i] = data.first[j];
					merged_intervals = true;
				}
				data.first[j] = -1;
				data.first[j + 1] = -1;
			}
			if (merged_intervals)
			{
				j = 0;
			}
		}
	}

	for (int i{ 0 }; i < data.first.size(); i += 2)
	{
		if (data.first[i] != -1)
		{
			result += data.first[i + 1] - data.first[i] + 1;
		}
	}


	return result;
}