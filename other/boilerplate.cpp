#include "../../other/include_everything.h"
import std;

void read_data();
int part_one();
int part_two();

int main()
{

	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 0.0000ms  : data in + parsing
	// 0.0000ms  : part1
	// 0.0000ms  : part2
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

void read_data()
{
	std::ifstream input(__INPUT_PATH);


}

int part_one()
{
	int result = 0;



	return result;
}

int part_two()
{
	int result = 0;



	return result;
}