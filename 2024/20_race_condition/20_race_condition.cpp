#include "../../other/include_everything.h"
import std;

typedef std::unordered_map<int, int> pos_with_time;
void run_program(std::string input, std::u8string comment, int min_time_saved);
void read_data(std::string input_path, int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, std::vector<int>& path);
int part_one(int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, int min_time_saved, std::vector<int>& path);
int part_two(int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, int min_time_saved, std::vector<int>& path);

int main()
{
	// 5.0176ms  : data in + parsing
	// 3.0800ms  : part1
	// 75.689ms  : part2
	run_program(__TEST_PATH, u8"test input", 1);
	run_program(__INPUT_PATH, u8"input day20", 100);
	return 0;
}

void run_program(std::string input, std::u8string comment, int min_time_saved)
{
	int start_pos, end_pos;
	std::vector<std::string> map;
	std::vector<int> path;
	pos_with_time positions_with_time;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, start_pos, end_pos, map, positions_with_time, path);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(start_pos, end_pos, map, positions_with_time, min_time_saved, path);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(start_pos, end_pos, map, positions_with_time, min_time_saved, path);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string input_path, int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, std::vector<int>& path)
{
	std::ifstream input(input_path);
	std::string temp;
	while (std::getline(input, temp))
	{
		map.emplace_back(temp);
	}
	for (int i{ 0 }; i != map.size(); ++i)
	{
		for (int j{ 0 }; j != map[0].size(); ++j)
		{
			if (map[i][j] == 'S') 
			{
				start_pos = 1000 * i + j;
				map[i][j] = '.';
			}
			if (map[i][j] == 'E') 
			{
				end_pos = 1000 * i + j;
				map[i][j] = '.';
			}
		}
	}
	int i{ 0 },cur_pos{ start_pos };
	for (;cur_pos != end_pos; ++i)
	{
		positions_with_time[cur_pos] = i;
		path.emplace_back(cur_pos);
		int x{ cur_pos / 1000 }, y{ cur_pos % 1000 };
		map[x][y] = ',';
		if (map[x][y + 1] == '.')
		{
			cur_pos = x * 1000 + ++y;
			continue;
		}
		if (map[x][y - 1] == '.')
		{
			cur_pos = x * 1000 + --y;
			continue;
		}
		if (map[x + 1][y] == '.')
		{
			cur_pos = ++x * 1000 + y;
			continue;
		}
		if (map[x - 1][y] == '.')
		{
			cur_pos = --x * 1000 + y;
			continue;
		}
	}
	positions_with_time[cur_pos] = i;
	path.emplace_back(cur_pos);
}

void find_cheats(int cur_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, std::unordered_set<int>& visited_before, std::unordered_map<int, int>& cheats, int cheat_distance, int min_cheat_time = 1)
{
	int x{ cur_pos / 1000 }, y{ cur_pos % 1000 };
	int v_min{ x - cheat_distance < 0 ? 0 : x - cheat_distance }, v_max{ x + cheat_distance + 1 > map.size() ? (int)map.size() : x + cheat_distance + 1 };
	int h_min{ y - cheat_distance < 0 ? 0 : y - cheat_distance }, h_max{ y + cheat_distance + 1 > map[0].size() ? (int)map[0].size() : y + cheat_distance + 1 };

	for (int i{ v_min }; i != v_max; ++i)
	{
		for (int j{ h_min }; j != h_max; ++j)
		{
			int moved_times{ std::abs(i - x) + std::abs(j - y) };
			if (moved_times > cheat_distance) continue;

			auto is_on_path = positions_with_time.find(i * 1000 + j);
			if (is_on_path == positions_with_time.end()) continue;
			if (visited_before.find((*is_on_path).first) != visited_before.end()) continue;
			int cheat_time{ (*is_on_path).second - positions_with_time[cur_pos] - moved_times };
			if (cheat_time >= min_cheat_time)
				++cheats[cheat_time];
		}
	}
}

int part_one(int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, int min_time_saved, std::vector<int>& path)
{
	int result{ 0 };
	std::unordered_set<int> visited_before;
	std::unordered_map<int, int> cheats;
	for (auto it{ path.begin() }; it != path.end(); ++it)
	{
		find_cheats(*it, map, positions_with_time, visited_before, cheats, 2, min_time_saved);
		visited_before.insert(*it);
	}

	for (auto it{ cheats.begin() }; it != cheats.end(); ++it)
	{
		result += (*it).second;
	}

	return result;
}

int part_two(int& start_pos, int& end_pos, std::vector<std::string>& map, pos_with_time& positions_with_time, int min_time_saved, std::vector<int>& path)
{
	int result{ 0 };
	std::unordered_set<int> visited_before;
	std::unordered_map<int, int> cheats;
	for (auto it{ path.begin() }; it != path.end(); ++it)
	{
		find_cheats(*it, map, positions_with_time, visited_before, cheats, 20, min_time_saved);
		visited_before.insert(*it);
	}

	for (auto it{ cheats.begin() }; it != cheats.end(); ++it)
	{
		result += (*it).second;
	}

	return result;
}