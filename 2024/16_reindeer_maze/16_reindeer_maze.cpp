#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, int &start_pos, int &end_pos, std::vector<std::string> &data);
int part_one(int& start_pos, int& end_pos, std::vector<std::string>& data);
int part_two(int& start_pos, int& end_pos, std::vector<std::string>& data);

int main()
{
	// 218.40µs  : data in + parsing
	// 13.500µs  : part1
	// 9.9000µs  : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day16");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::vector<std::string> data;
	int start_pos, end_pos;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, start_pos, end_pos, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(start_pos, end_pos, data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(start_pos, end_pos, data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, int& start_pos, int& end_pos, std::vector<std::string>& data)
{
	std::ifstream input(input_path);
	std::string temp;
	while (std::getline(input, temp))
	{
		data.emplace_back(temp);
	}
	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ 0 }; j < data[0].size(); ++j)
		{
			if (data[i][j] == 'S')
				start_pos = i * 1000 + j;
			if(data[i][j] == 'E')
				end_pos = i * 1000 + j;
		}
	}
}
int get_multiplier(int new_dir, int cur_dir)
{
	switch (new_dir * 10 + cur_dir)
	{
	case 11:
	case 22:
	case 33:
	case 44:
		return 0;
	case 12:
	case 23:
	case 34:
	case 41:
	case 21:
	case 32:
	case 43:
	case 14:
		return 1;
	case 13:
	case 31:
	case 24:
	case 42:
		return 2;
	}
}

void print_map(std::vector<std::vector<int>>& data)
{
	int width{ 10 };
	std::ofstream output(__OUT_PATH);
	for (auto row : data)
	{
		for (auto i : row)
		{
			if(i == INT_MAX)
				output << std::setw(width) << -1;
			else if (i == INT_MIN)
				output << std::setw(width) << "#######";
			else if (i < 0)
				output << std::setw(width) << "[" + std::to_string(-i) + "]";
			else
				output << std::setw(width) << i;
		}
		output << '\n';
	}
	output << '\n';
	output.close();
}

int fill_maze(int& start_pos, int& end_pos, std::vector<std::string>& data, std::vector<std::vector<int>> &visited)
{
	enum dir { north = 1, west = 2, south = 3, east = 4 };
	for (int i{ 0 }; i != data.size(); ++i)
	{
		for (int j{ 0 }; j != data[0].size(); ++j)
		{
			if (data[i][j] == '#')
				visited[i][j] = INT_MIN;
			else
				visited[i][j] = INT_MAX;
		}
	}
	std::queue<std::tuple<int, int, dir>> to_search;
	to_search.emplace(std::tuple<int, int, dir>{ start_pos, 0, dir::east });
	std::vector<int> results;
	while (!to_search.empty())
	{
		int x{ std::get<0>(to_search.front()) / 1000 }, y{ std::get<0>(to_search.front()) % 1000 }, score{ std::get<1>(to_search.front()) };

		if (std::get<0>(to_search.front()) == end_pos)
		{
			results.emplace_back(std::get<1>(to_search.front()));
			if (score < visited[x][y])
				visited[x][y] = score;
			to_search.pop();
			continue;
		}
		dir cur_dir{ std::get<2>(to_search.front()) };

		 if (visited[x][y] < score)
		{
			if (score - 1000 == visited[x][y])
			{
				if (visited[x][y - 1] == INT_MAX || visited[x][y + 1] == INT_MAX || visited[x - 1][y] == INT_MAX || visited[x + 1][y] == INT_MAX)
				{
					to_search.emplace(to_search.front());
					to_search.pop();
					continue;
				}
				if (std::abs(visited[x][y - 1] - visited[x - 1][y]) == std::abs(visited[x][y + 1] - visited[x + 1][y]))
				{
					to_search.pop();
					continue;
				}
				bool check_west = std::abs(visited[x][y - 1] - score) == 1;
				bool check_east = std::abs(visited[x][y + 1] - score) == 1;
				bool check_north = std::abs(visited[x - 1][y] - score) == 1;
				bool check_south = std::abs(visited[x + 1][y] - score) == 1;
				bool conclusion = false;
				switch (cur_dir)
				{
				case dir::north:
					conclusion = check_north || check_east || check_west;
					break;
				case dir::west:
					conclusion = check_south || check_west || check_north;
					break;
				case dir::south:
					conclusion = check_south || check_east || check_west;
					break;
				case dir::east:
					conclusion = check_south || check_north || check_east;
					break;
				}
				if(conclusion)
					visited[x][y] = score;
			}
			to_search.pop();
			continue;
		}
		else if (score + 1000 == visited[x][y])
		{
			visited[x][y] = score + 1000;
		}
		else
		{
			visited[x][y] = score;
		}
		to_search.pop();
		if (visited[x + 1][y] > -1 && get_multiplier(dir::south, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ (x + 1) * 1000 + y, score + 1000 * get_multiplier(dir::south, cur_dir) + 1, dir::south });
		}
		if (visited[x][y + 1] > -1 && get_multiplier(dir::east, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y + 1, score + 1000 * get_multiplier(dir::east, cur_dir) + 1, dir::east });
		}
		if (visited[x - 1][y] > -1 && get_multiplier(dir::north, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ (x - 1) * 1000 + y, score + 1000 * get_multiplier(dir::north, cur_dir) + 1, dir::north });
		}
		if (visited[x][y - 1] > -1 && get_multiplier(dir::west, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y - 1, score + 1000 * get_multiplier(dir::west, cur_dir) + 1, dir::west });
		}
	}
	return *std::min_element(results.begin(), results.end());
}

int part_one(int& start_pos, int& end_pos, std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<std::vector<int>> visited{ std::vector<std::vector<int>>(data.size(), std::vector<int>(data[0].size(), 0)) };
	result = fill_maze(start_pos, end_pos, data, visited);
	return result;
}

bool possible_cross(std::vector<std::vector<int>>& visited, int x, int y)
{
	if (std::abs(std::abs(visited[x - 1][y]) - std::abs(visited[x + 1][y])) == 2 && std::abs(std::abs(visited[x][y - 1]) - std::abs(visited[x][y + 1])) == 2)
		return true;

	return false;
}

void reverse_fill(int& start_pos, int& end_pos, std::vector<std::vector<int>>& visited)
{
	enum dir { north = 1, west = 2, south = 3, east = 4 };
	std::queue<std::tuple<int, int, dir>> to_search;
	to_search.emplace(std::tuple<int, int, dir>{ end_pos, visited[end_pos / 1000][end_pos % 1000], dir::south });
	while (!to_search.empty())
	{
		int x{ std::get<0>(to_search.front()) / 1000 }, y{ std::get<0>(to_search.front()) % 1000 }, score{ std::get<1>(to_search.front()) };
		dir cur_dir{ std::get<2>(to_search.front()) };
		if (std::get<0>(to_search.front()) == start_pos)
		{
			visited[x][y] = -1;
		}

		if (std::get<0>(to_search.front()) == start_pos || visited[x][y] < score)
		{
			to_search.pop();
			continue;
		}

		visited[x][y] = -visited[x][y];

		to_search.pop();
		bool valid_north{ false };
		bool valid_south{ false };
		bool valid_east{ false };
		bool valid_west{ false };
		if (get_multiplier(dir::north, cur_dir) < 2)
		{
			if (get_multiplier(dir::north, cur_dir) == 0)
				valid_north = score - visited[x - 1][y] - 1000 == 1 || score - visited[x - 1][y] == 1 || score - visited[x - 1][y] + 1000 == 1;
			else
				valid_north = score - visited[x - 1][y] - 1000 == 1 || score - visited[x - 1][y] == 1;
		}
		if (get_multiplier(dir::south, cur_dir) < 2)
		{
			if (get_multiplier(dir::south, cur_dir) == 0)
				valid_south = score - visited[x + 1][y] - 1000 == 1 || score - visited[x + 1][y] == 1 || score - visited[x + 1][y] + 1000 == 1;
			else
				valid_south = score - visited[x + 1][y] - 1000 == 1 || score - visited[x + 1][y] == 1;
		}
		if (get_multiplier(dir::east, cur_dir) < 2)
		{
			if (get_multiplier(dir::east, cur_dir) == 0)
				valid_east = score - visited[x][y + 1] - 1000 == 1 || score - visited[x][y + 1] == 1 || score - visited[x][y + 1] + 1000 == 1;
			else
				valid_east = score - visited[x][y + 1] - 1000 == 1 || score - visited[x][y + 1] == 1;
		}
		if (get_multiplier(dir::west, cur_dir) < 2)
		{
			if (get_multiplier(dir::west, cur_dir) == 0)
				valid_west = score - visited[x][y - 1] - 1000 == 1 || score - visited[x][y - 1] == 1 || score - visited[x][y - 1] + 1000 == 1;
			else
				valid_west = score - visited[x][y - 1] - 1000 == 1 || score - visited[x][y - 1] == 1;
		}
		if (visited[x + 1][y] > -1 && valid_south)
		{
			to_search.emplace(std::tuple<int, int, dir>{ (x + 1) * 1000 + y, visited[x + 1][y], dir::south });
		}
		if (visited[x][y + 1] > -1 && valid_east)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y + 1, visited[x][y + 1], dir::east });
		}
		if (visited[x - 1][y] > -1 && valid_north)
		{
			if(score != 20078)
				to_search.emplace(std::tuple<int, int, dir>{ (x - 1) * 1000 + y, visited[x - 1][y], dir::north });
		}
		if (visited[x][y - 1] > -1 && valid_west)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y - 1, visited[x][y - 1], dir::west });

		}
	}
	//print_map(visited);
}

int part_two(int& start_pos, int& end_pos, std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<std::vector<int>> visited{ std::vector<std::vector<int>>(data.size(), std::vector<int>(data[0].size(), 0)) };
	fill_maze(start_pos, end_pos, data, visited);
	reverse_fill(start_pos, end_pos, visited);
	for (auto& row : visited)
		for (auto n : row)
			if (n != INT_MIN && n < 0)
				++result;

	return result;
}