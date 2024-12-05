#include "../../other/include_everything.h"
import std;

void read_data(std::array<std::vector<int>, 100>& rules, std::vector<std::vector<int>>& sequences);
int part_one(std::array<std::vector<int>, 100>& rules, std::vector<std::vector<int>>& sequences);
int part_two(std::array<std::vector<int>, 100>& rules, std::vector<std::vector<int>>& sequences);
std::pair<int, int>  check_sequence(std::array<std::vector<int>, 100>& rules, std::vector<int>& sequence);
int main()
{
	std::vector<std::vector<int>> sequences;
	std::array<std::vector<int>, 100> rules;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 1.4694ms  : data in + parsing
	// 0.0413ms  : part1
	// 3.5266ms  : part2
	input_timer.startTimer();
	read_data(rules, sequences);
	input_timer.getDuration();

	part1_timer.startTimer();
	part_one_result = part_one(rules, sequences);
	part1_timer.getDuration();

	part2_timer.startTimer();
	part_two_result = part_two(rules, sequences);
	part2_timer.getDuration();

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::array<std::vector<int>, 100> &rules, std::vector<std::vector<int>>&sequences)
{
	std::ifstream input(__INPUT_PATH);
	std::string line;
	int temp1, temp2;
	char temp_c;

	for (std::getline(input, line); line != ""; std::getline(input, line))
	{
		std::stringstream line_in(line);
		line_in >> temp1 >> temp_c >> temp2;

		rules[temp2].emplace_back(temp1);
	}

	while (std::getline(input, line))
	{
		std::stringstream line_in(line);
		std::vector<int> sequence;
		while (!line_in.eof())
		{
			line_in >> temp1;
			sequence.emplace_back(temp1);
			line_in >> temp_c;
		}
		sequences.emplace_back(sequence);
	}
}

int part_one(std::array<std::vector<int>, 100>& rules, std::vector<std::vector<int>>& sequences)
{
	int result = 0;

	for (int i{ 0 }; i != sequences.size(); ++i)
	{
		if (check_sequence(rules, sequences[i]).first == -1)
			result += sequences[i][sequences[i].size() / 2];
	}

	return result;
}

int part_two(std::array<std::vector<int>, 100>& rules, std::vector<std::vector<int>>& sequences)
{
	int result = 0;

	for (int i{ 0 }; i != sequences.size(); ++i)
	{
		if (check_sequence(rules, sequences[i]).first != -1)
		{
			while (check_sequence(rules, sequences[i]).first != -1)
			{
				auto [idx1, idx2] = check_sequence(rules, sequences[i]);
				std::swap(sequences[i][idx1], sequences[i][idx2]);
			}
			result += sequences[i][sequences[i].size() / 2];
		}
	}

	return result;
}

std::pair<int, int> check_sequence(std::array<std::vector<int>, 100>& rules, std::vector<int>& sequence)
{
	for (int i{ 0 }; i != sequence.size(); ++i)
	{
		for (int j{ i + 1 }; j != sequence.size(); ++j)
		{
			if (std::find(rules[sequence[i]].begin(), rules[sequence[i]].end(), sequence[j]) != rules[sequence[i]].end())
				return std::pair(i, j);
		}
	}

	return std::pair(-1, -1);
}