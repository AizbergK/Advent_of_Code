#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment, std::pair<int, int> map_size, int bytes);
void read_data(std::string& input_path, std::vector<std::pair<int, int>>& data);
int part_one(std::vector<std::pair<int, int>>& data,std::pair<int, int> map_size, int bytes);
int part_two(std::vector<std::pair<int, int>>& data, std::pair<int, int> map_size, int bytes);

int main()
{
	// 442.90µs  : data in + parsing
	// 93.500µs  : part1
	// 139.31ms  : part2
	run_program(__TEST_PATH, u8"test input", { 7, 7 }, 12);
	run_program(__INPUT_PATH, u8"input day18", { 71, 71 }, 1024);
	return 0;
}

void run_program(std::string input, std::u8string comment, std::pair<int, int> map_size, int bytes)
{
	std::vector<std::pair<int, int>> data;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data, map_size, bytes);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data, map_size, bytes);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<std::pair<int, int>>& data)
{
	std::ifstream input(input_path);
	std::string temp;
	int temp_i1, temp_i2;
	while (std::getline(input, temp))
	{
		temp_i1 = std::stoi(temp);
		temp = temp.substr(temp.find_first_of(',') + 1);
		temp_i2 = std::stoi(temp);
		data.emplace_back(std::pair<int, int>{ temp_i1, temp_i2 });
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

int fill_maze(int start_pos, int end_pos, std::vector<std::vector<int>>& visited)
{
	enum dir { north = 1, west = 2, south = 3, east = 4 };

	std::queue<std::tuple<int, int, dir>> to_search;
	to_search.emplace(std::tuple<int, int, dir>{ start_pos, 0, dir::east });
	while (!to_search.empty())
	{
		int x{ std::get<0>(to_search.front()) / 1000 }, y{ std::get<0>(to_search.front()) % 1000 }, score{ std::get<1>(to_search.front()) };

		if (std::get<0>(to_search.front()) == end_pos)
		{
			return score;
		}
		dir cur_dir{ std::get<2>(to_search.front()) };

		if (visited[x][y] <= score)
		{
			to_search.pop();
			continue;
		}

		visited[x][y] = score;

		to_search.pop();
		if (x + 1 <= end_pos / 1000 && visited[x + 1][y] > -1 && get_multiplier(dir::south, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ (x + 1) * 1000 + y, score + 1, dir::south });
		}
		if (y + 1 <= end_pos / 1000 && visited[x][y + 1] > -1 && get_multiplier(dir::east, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y + 1, score + 1, dir::east });
		}
		if (x - 1 >= 0 && visited[x - 1][y] > -1 && get_multiplier(dir::north, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ (x - 1) * 1000 + y, score + 1, dir::north });
		}
		if (y - 1 >= 0 && visited[x][y - 1] > -1 && get_multiplier(dir::west, cur_dir) < 2)
		{
			to_search.emplace(std::tuple<int, int, dir>{ x * 1000 + y - 1, score + 1, dir::west });
		}
	}
	return 0;
}

int part_one(std::vector<std::pair<int, int>>& data, std::pair<int, int> map_size, int bytes)
{
	int final_coord{ (map_size.first - 1) * 1000 + map_size.second - 1 };
	std::vector<std::vector<int>> map{ std::vector<std::vector<int>>{(std::size_t)map_size.first, std::vector<int>(map_size.second, INT_MAX)} };
	for (int i{ 0 }; i != bytes; ++i)
		map[data[i].first][data[i].second] = INT_MIN;
	return fill_maze(0, final_coord, map);
}

int part_two(std::vector<std::pair<int, int>>& data, std::pair<int, int> map_size, int bytes)
{
	int final_coord{ (map_size.first - 1) * 1000 + map_size.second - 1 };

	for (int i{ 0 }; i != data.size(); ++i)
	{
		std::vector<std::vector<int>> map{ std::vector<std::vector<int>>{(std::size_t)map_size.first, std::vector<int>(map_size.second, INT_MAX)} };
		for (int j{ 0 }; j != i + 1; ++j)
			map[data[j].first][data[j].second] = INT_MIN;
		int result{ fill_maze(0, final_coord, map) };
		if (!result)
		{
			std::cout << data[i].first << ',' << data[i].second << '\n';
			break;
		}
	}
	return 0;
}