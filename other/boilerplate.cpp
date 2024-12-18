#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path);
int part_one();
int part_two();

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
	
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one();
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two();
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string &input_path)
{
	std::ifstream input(input_path);


}

int part_one()
{
	int result{ 0 };



	return result;
}

int part_two()
{
	int result{ 0 };



	return result;
}