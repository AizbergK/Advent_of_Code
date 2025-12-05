#include "../../other/include_everything.h"
import std;

typedef int data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 000000ms  : data in + parsing
	// 000000ms  : part1
	// 000000ms  : part2
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

void read_data(std::string &input_path, data_type& data)
{
	std::ifstream input(input_path);


}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };



	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 0 };



	return result;
}