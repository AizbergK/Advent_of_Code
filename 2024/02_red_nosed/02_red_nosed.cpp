#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::vector<int>> &data);
int part_one(std::vector<std::vector<int>>& data);
int part_two(std::vector<std::vector<int>>& data);
std::pair<int,int> safety_check(std::vector<int> row);

int main()
{
	std::vector<std::vector<int>> data;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 1.7310ms  : data in + parsing
	// 0.0422ms  : part1
	// 0.3668ms  : part2
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

void read_data(std::vector<std::vector<int>>& data)
{
	std::ifstream input(__INPUT_PATH);
	std::string line;
	int line_count = 0, temp;
	while (!input.eof())
	{
		std::getline(input, line);
		std::stringstream ss(line);
		data.emplace_back(std::vector<int>{});
		while (!ss.eof())
		{
			ss >> temp;
			data[line_count].emplace_back(temp);
		}
		++line_count;
	}

}

int part_one(std::vector<std::vector<int>>& data)
{
	int result = 0;

	for (int i{ 0 }; i != data.size(); ++i)
		result += safety_check(data[i]).first == -1 ? 1 : 0;

	return result;
}

int part_two(std::vector<std::vector<int>>& data)
{
	int result = 0, pos, type;
	for (int i{ 0 }; i != data.size(); ++i)
	{
		auto [pos, type] = safety_check(data[i]);
		if (pos == -1)
		{
			++result;
		}
		else
		{
			std::vector<int> rm_prev = data[i], rm_cur = data[i], rm_next = data[i];
			if (type == 1)
				rm_prev.erase(rm_prev.begin() + pos - 1);
			rm_cur.erase(rm_cur.begin() + pos);
			rm_next.erase(rm_next.begin() + pos + 1);
			if (safety_check(rm_cur).first == -1 || safety_check(rm_next).first == -1 || safety_check(rm_prev).first == -1)
			{
				++result;
			}
		}
	}

	return result;
}

std::pair<int, int> safety_check(std::vector<int> row)
{
	bool is_ascending = row[0] < row[1];

	for (int i{ 0 }; i != row.size() - 1; ++i)
	{
		if (is_ascending && row[i] > row[i + 1]) return { i, 1 };
		if (!is_ascending && row[i] < row[i + 1]) return { i, 1 };
		if (3 < std::abs(row[i] - row[i + 1]) || std::abs(row[i] - row[i + 1]) < 1) return { i, 2 };
	}

	return std::pair<int, int>{ - 1, 0 };
}