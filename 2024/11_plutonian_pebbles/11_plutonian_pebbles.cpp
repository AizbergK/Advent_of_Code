#include "../../other/include_everything.h"
import std;

void read_data(std::map<long long, long long>& data);
long long part_one(std::map<long long, long long> data);
long long part_two(std::map<long long, long long> data);
long long get_digit_count(long long number);

int main()
{
	std::map<long long, long long> data;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 204.70µs  : data in + parsing
	// 141.30µs  : part1
	// 214.60s   : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input day11");

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::map<long long, long long>& data)
{
	std::ifstream input(__INPUT_PATH);
	int temp;
	while (input >> temp)
	{
		data[temp] = 1;
	}
}

long long get_digit_count(long long number)
{
	long long count = 0;
	do
	{
		++count;
		number /= 10;
	} while (number);

	return count;
}

long long do_steps(std::map<long long, long long> data, int steps)
{
	long long result = 0;
	for (int i{ 0 }; i != steps; ++i)
	{
		std::map<long long, long long> temp_data;
		for (auto it{ data.begin() }; it != data.end(); ++it)
		{
			if (it->first == 0)
			{
				temp_data[1] += it->second;
				continue;
			}
			auto digits{ get_digit_count(it->first) };
			if (digits % 2 == 1)
			{
				temp_data[it->first * 2024] += it->second;
			}
			else
			{
				long long left{ it->first / (long long)std::pow(10, digits / 2) }, right{ it->first % (long long)std::pow(10, digits / 2) };
				temp_data[left] += it->second;
				temp_data[right] += it->second;
			}
		}
		data.clear();
		data = temp_data;
	}

	for (auto it{ data.begin() }; it != data.end(); ++it)
		result += it->second;

	return result;
}

long long part_one(std::map<long long, long long> data)
{
	long long result = 0;

	return do_steps(data, 25);
}

long long part_two(std::map<long long, long long> data)
{
	long long result = 0;

	return do_steps(data, 75);
}