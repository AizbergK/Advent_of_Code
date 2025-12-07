#include "../../other/include_everything.h"
import std;

typedef std::vector<std::string> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 178.80µs : input input day15
	// 16.100µs : part1
	// 14.800µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day15");
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
		data.emplace_back(line);
		std::getline(input, line);
	}
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };

	std::vector<int> beam_memo(data[0].size(), 0);
	beam_memo[data[0].size() / 2] = 1;

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ 0 }; j < data[0].size(); ++j)
		{
			if (data[i][j] == '^' && beam_memo[j])
			{
				++result;
				beam_memo[j - 1] += beam_memo[j];
				beam_memo[j + 1] += beam_memo[j];
				beam_memo[j] = 0;
			}
		}
	}

	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 0 };

	std::vector<std::uint64_t> beam_memo(data[0].size(), 0);
	beam_memo[data[0].size() / 2] = 1;

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ 0 }; j < data[0].size(); ++j)
		{
			if (data[i][j] == '^' && beam_memo[j])
			{
				beam_memo[j - 1] += beam_memo[j];
				beam_memo[j + 1] += beam_memo[j];
				beam_memo[j] = 0;
			}
		}
	}

	result = std::accumulate(beam_memo.begin(), beam_memo.end(), 0ULL);

	return result;
}