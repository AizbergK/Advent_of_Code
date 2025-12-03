#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::int16_t>& data);
int part_one(std::vector<std::int16_t>& data);
int part_two(std::vector<std::int16_t>& data);

int main()
{
	// 1.1023ms : input input day01
	// 4.3000µs : part1
	// 6.5000µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day01");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	std::vector<std::int16_t> data;

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

void read_data(std::string& input_path, std::vector<std::int16_t> &data)
{
	std::ifstream input(input_path);

	char in;
	std::int16_t num;

	while (input >> in, input >> num)
	{
		data.emplace_back(in == 'R' ? num : -num);
	}
}

int part_one(std::vector<std::int16_t>& data)
{
	int result{ 0 };

	std::int32_t position{ INT_MAX / 200 * 100 + 50 };
	for (const std::int16_t step : data)
	{
		position += step;
		result += position % 100 == 0;
	}

	return result;
}

int part_two(std::vector<std::int16_t>& data)
{
	int result{ 0 };

	std::int32_t position{ INT_MAX / 200 * 100 + 50 };

	for (const std::int16_t step : data)
	{
		const int prev_position = position;
		position += step;
		const bool sub_val{ step < 0 };
		const int prev_div = (prev_position - sub_val) / 100;
		const int curr_div = (position - sub_val) / 100;
		result += std::abs(curr_div - prev_div);
	}

	return result;
}