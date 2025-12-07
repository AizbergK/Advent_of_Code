#include "../../other/include_everything.h"
import std;

typedef std::vector<std::uint8_t> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 142.10µs : input input day07
	// 1.1000µs : part1
	// 1.2000µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day07");
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

	std::string line{  };
	std::getline(input, line);
	std::getline(input, line);
	while (std::getline(input, line))
	{
		for (int i{ 0 }; i < line.size(); ++i)
		{
			if (line[i] == '^') data.emplace_back(i);
		}
		std::getline(input, line);
	}
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };

	std::array<std::uint8_t, 141> beam_memo = { 0 };
	beam_memo[data[0]] = 1;

	for (auto pos : data)
	{
		result += beam_memo[pos];
		beam_memo[pos - 1] |= beam_memo[pos];
		beam_memo[pos + 1] |= beam_memo[pos];
		beam_memo[pos] = 0;
	}

	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 1 };

	std::array<std::uint64_t, 141> beam_memo = { 0 };
	beam_memo[data[0]] = 1;

	for (auto pos : data)
	{
		result += beam_memo[pos];
		beam_memo[pos - 1] += beam_memo[pos];
		beam_memo[pos + 1] += beam_memo[pos];
		beam_memo[pos] = 0;
	}

	return result;
}