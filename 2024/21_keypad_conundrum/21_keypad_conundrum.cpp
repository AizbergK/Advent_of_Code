#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::string>& data);
int part_one(std::vector<std::string>& data);
int part_two(std::vector<std::string>& data);

int main()
{
	// 000000ms  : data in + parsing
	// 000000ms  : part1
	// 000000ms  : part2
	//run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day21");
	return 0;
}

constexpr std::array<std::array<char, 3>, 4> keypad
{
	std::array<char, 3>{ '7', '8', '9' },
	std::array<char, 3>{ '4', '5', '6' },
	std::array<char, 3>{ '1', '2', '3' },
	std::array<char, 3>{ '.', '0', 'A' }
};

constexpr std::array<std::array<char, 3>, 2> arrpad
{
	std::array<char, 3>{ '.', '^', 'A' },
	std::array<char, 3>{ '<', 'v', '>' }
};

void run_program(std::string input, std::u8string comment)
{
	std::vector<std::string> data;

	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

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

void read_data(std::string& input_path, std::vector<std::string>& data)
{
	std::ifstream input(input_path);
	std::string temp;
	while (std::getline(input, temp))
	{
		data.emplace_back(temp);
	}
}

std::pair<int, int> get_pos(char ch)
{
	switch (ch)
	{
	case 'A': return { 3, 2 };
	case '0': return { 3, 1 };
	case '1': return { 2, 0 };
	case '2': return { 2, 1 };
	case '3': return { 2, 2 };
	case '4': return { 1, 0 };
	case '5': return { 1, 1 };
	case '6': return { 1, 2 };
	case '7': return { 0, 0 };
	case '8': return { 0, 1 };
	case '9': return { 0, 2 };
	}
}

char get_pos(int pos)
{
	switch (pos)
	{
	case 32: return 'A';
	case 31: return '0';
	case 20: return '1';
	case 21: return '2';
	case 22: return '3';
	case 10: return '4';
	case 11: return '5';
	case 12: return '6';
	case 00: return '7';
	case 01: return '8';
	case 02: return '9';
	}
}

void find_keypad_paths(char to_ch, std::queue<std::pair<char, std::string>>& paths)
{
	while(paths.front().first != to_ch)
	{
		std::pair<int, int> from{ get_pos(paths.front().first) };
		std::pair<int, int> to{ get_pos(to_ch) };
		if (std::abs(from.first - to.first) > std::abs(from.first + 1 - to.first) && keypad[from.first + 1][from.second] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos((from.first + 1) * 10 + from.second), paths.front().second + 'v' });
		}
		if (std::abs(from.first - to.first) > std::abs(from.first - 1 - to.first) && keypad[from.first - 1][from.second] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos((from.first - 1) * 10 + from.second), paths.front().second + '^' });
		}
		if (std::abs(from.second - to.second) > std::abs(from.second - to.second + 1) && keypad[from.first][from.second + 1] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos(from.first * 10 + from.second + 1), paths.front().second + '>' });
		}
		if (std::abs(from.second - to.second) > std::abs(from.second - to.second - 1) && keypad[from.first][from.second - 1] != '.')
		{
			paths.push(std::pair<char, std::string>{ get_pos(from.first * 10 + from.second - 1), paths.front().second + '<' });
		}
		paths.pop();

	}
}

std::pair<int, int> get_pos_arrpad(char ch)
{
	switch (ch)
	{
	case 'A': return { 0, 2 };
	case '^': return { 0, 1 };
	case '<': return { 1, 0 };
	case 'v': return { 1, 1 };
	case '>': return { 1, 2 };
	}
}

char get_pos_arrpad(int pos)
{
	switch (pos)
	{
	case 02: return 'A';
	case 01: return '^';
	case 10: return '<';
	case 11: return 'v';
	case 12: return '>';
	}
}

void find_arrpad_paths(char to_ch, std::queue<std::pair<char, std::string>>& paths)
{
	while (!paths.empty() && paths.front().first != to_ch)
	{
		std::pair<int, int> from{ get_pos_arrpad(paths.front().first) };
		std::pair<int, int> to{ get_pos_arrpad(to_ch) };
		if (std::abs(from.first - to.first) > std::abs(from.first + 1 - to.first) && arrpad[from.first + 1][from.second] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos_arrpad((from.first + 1) * 10 + from.second), paths.front().second + 'v' });
		}
		if (std::abs(from.first - to.first) > std::abs(from.first - 1 - to.first) && arrpad[from.first - 1][from.second] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos_arrpad((from.first - 1) * 10 + from.second), paths.front().second + '^' });
		}
		if (std::abs(from.second - to.second) > std::abs(from.second - to.second + 1) && arrpad[from.first][from.second + 1] != '.')
		{
			paths.emplace(std::pair<char, std::string>{ get_pos_arrpad(from.first * 10 + from.second + 1), paths.front().second + '>' });
		}
		if (std::abs(from.second - to.second) > std::abs(from.second - to.second - 1) && arrpad[from.first][from.second - 1] != '.')
		{
			paths.push(std::pair<char, std::string>{ get_pos_arrpad(from.first * 10 + from.second - 1), paths.front().second + '<' });
		}
		paths.pop();
	}
}

int arrpad_robots(int cur_depth, int max_depth,char prev, char next)
{
	if (cur_depth == max_depth) return 1;
	std::queue<std::pair<char, std::string>> paths_queue;

	paths_queue.push(std::pair<char, std::string>{prev, ""});
	find_arrpad_paths(next, paths_queue);
	std::vector<std::pair<char, std::string>> paths;
	while (!paths_queue.empty())
	{
		paths.emplace_back(paths_queue.front());
		paths_queue.pop();
		paths.rbegin()->second += 'A';
	}
	std::vector<int> path_costs(paths.size(), 0);

	char arrpad_prev{ 'A' };
	for (int path_no{ 0 }; path_no != paths.size(); ++path_no)
	{
		for (int j{ 0 }; j != paths[path_no].second.size(); ++j)
		{
			path_costs[path_no] += arrpad_robots(cur_depth + 1, max_depth, arrpad_prev, paths[path_no].second[j]);
			arrpad_prev = paths[path_no].second[j];
		}
	}

	return *std::min_element(path_costs.begin(), path_costs.end());
}

void keypad_robot(std::vector<std::string>& data, int max_depth, std::vector<int>& steps_for_each_row)
{
	char prev{ 'A' };
	std::vector<int> shortest_sequences;
	for (auto& row : data)
	{
		std::vector<int> step_costs(10, 0);
		int i{ 0 };
		for (i; i != row.size(); ++i)
		{
			std::queue<std::pair<char, std::string>> paths_queue;
			paths_queue.push(std::pair<char, std::string>{prev, ""});
			find_keypad_paths(row[i], paths_queue);
			std::vector<std::pair<char, std::string>>paths;
			while (!paths_queue.empty())
			{
				paths.emplace_back(paths_queue.front());
				paths_queue.pop();
				paths.rbegin()->second += 'A';
			}
			std::vector<int> path_costs(paths.size(), 0);
			prev = row[i];

			char arrpad_prev{ 'A' };
			for (int path_no{ 0 }; path_no != paths.size(); ++path_no)
			{
				for (int j{ 0 }; j != paths[path_no].second.size(); ++j)
				{
					path_costs[path_no] += arrpad_robots(0, max_depth, arrpad_prev, paths[path_no].second[j]);
					arrpad_prev = paths[path_no].second[j];
				}
			}
			step_costs[i] = *std::min_element(path_costs.begin(), path_costs.end());
		}
		steps_for_each_row.emplace_back(std::accumulate(step_costs.begin(), step_costs.end(), 0));
	}
}

int part_one(std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<int> steps_for_each_row;
	keypad_robot(data, 2, steps_for_each_row);
	for (int i{ 0 }; i != data.size(); ++i)
		result += std::stoi(data[i]) * steps_for_each_row[i];
	return result;
}

int part_two(std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<int> steps_for_each_row;
	keypad_robot(data, 25, steps_for_each_row);
	for (int i{ 0 }; i != data.size(); ++i)
		result += std::stoi(data[i]) * steps_for_each_row[i];
	return result;
}