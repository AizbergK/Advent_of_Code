#include "../../other/include_everything.h"
import std;

void read_data(std::vector<int>& left, std::vector<int>& right);
int part_one(std::vector<int>& left, std::vector<int>& right);
int part_two(std::vector<int>& left, std::vector<int>& right);

int main()
{
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 0.5764ms  : data in + parsing
	// 0.0654ms  : part1
	// 0.0599ms  : part2

	std::vector<int> left, right;
	input_timer.startTimer();
	read_data(left, right);
	input_timer.getDuration();

	part1_timer.startTimer();
	part_one_result = part_one(left, right);
	part1_timer.getDuration();

	part2_timer.startTimer();
	part_two_result = part_two(left, right);
	part2_timer.getDuration();

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);

	return 0;
}

void read_data(std::vector<int>& left, std::vector<int>& right)
{
	std::ifstream input(__INPUT_PATH);
	int temp;
	while (!input.eof())
	{
		input >> temp;
		left.emplace_back(temp);
		input >> temp;
		right.emplace_back(temp);
	}
}

int part_one(std::vector<int>& left, std::vector<int>& right)
{
	int result{ 0 };
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());
	for (int i{ 0 }; i != left.size(); ++i)
	{
		result += std::abs(left[i] - right[i]);
	}

	return result;
}

int part_two(std::vector<int>& left, std::vector<int>& right)
{
	int result{ 0 };
	std::map<int, int> similarity_map;
	for (int i{ 0 }; i != left.size(); ++i)
	{
		if (find(left.begin(), left.end(), right[i]) != left.end())
		{
			++similarity_map[right[i]];
		}
	}
	for (int i{ 0 }; i != left.size(); ++i)
	{
		if (similarity_map.find(left[i]) != similarity_map.end())
		{
			result += left[i] * similarity_map[left[i]];
		}
	}

	return result;
}