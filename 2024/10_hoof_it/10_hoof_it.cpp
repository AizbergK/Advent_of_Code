#include "../../other/include_everything.h"
import std;

typedef std::vector<std::string> data_type;
typedef std::queue<std::pair<int, int>> search_queue;
typedef std::vector<int> positions;

void read_data(data_type& data);
int part_one(data_type& data);
int part_two(data_type& data);
void search_all(int start_pos, data_type& data, search_queue& to_search, positions& paths);
void search_all_pt2(int start_pos, data_type& data, search_queue& to_search, positions& paths);

int main()
{
	data_type data;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 149.30µs  : data in + parsing
	// 240.40µs  : part1
	// 203.30µs  : part2

	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input day10");

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);

	return 0;
}

void read_data(data_type& data)
{
	std::ifstream input(__INPUT_PATH);
	std::string temp;
	while (std::getline(input, temp))
		data.emplace_back(temp);
}

int part_one(data_type& data)
{
	positions start_poses;
	positions end_poses;
	positions paths;
	for (int i{ 0 }; i != data[0].size(); ++i)
	{
		for (int j{ 0 }; j != data.size(); ++j)
		{
			if (data[i][j] == '0')
				start_poses.emplace_back(i * 100 + j);
			if (data[i][j] == '9')
				end_poses.emplace_back(i * 100 + j);
		}
	}
	for (int s{ 0 }; s != start_poses.size(); ++s)
	{
		int start_pos{ start_poses[s] };
		search_queue to_search;

		to_search.push(std::pair<int, int>{ start_pos / 100, start_pos % 100 });
		while (!to_search.empty())
		{
			search_all(start_pos, data, to_search, paths);
		}
	}

	return paths.size();
}

void search_all(int start_pos, data_type& data, search_queue& to_search, positions& paths)
{
	std::pair<int, int> search_pos{ to_search.front() };
	if (data[search_pos.first][search_pos.second] == '9')
	{
		int path = start_pos * 10000 + search_pos.first * 100 + search_pos.second;
		if(std::find(paths.begin(), paths.end(), path) == paths.end())
			paths.emplace_back(path);
		to_search.pop();
		return;
	}
	to_search.pop();

	if (search_pos.first < data.size() - 1 && data[search_pos.first + 1][search_pos.second] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first + 1, search_pos.second });
	if (search_pos.first > 0 && data[search_pos.first - 1][search_pos.second] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first - 1, search_pos.second });
	if (search_pos.second < data[0].size() - 1 && data[search_pos.first][search_pos.second + 1] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first, search_pos.second + 1 });
	if (search_pos.second > 0 && data[search_pos.first][search_pos.second - 1] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first, search_pos.second - 1 });
}

int part_two(data_type& data)
{
	positions start_poses;
	positions end_poses;
	positions paths;
	for (int i{ 0 }; i != data[0].size(); ++i)
	{
		for (int j{ 0 }; j != data.size(); ++j)
		{
			if (data[i][j] == '0')
				start_poses.emplace_back(i * 100 + j);
			if (data[i][j] == '9')
				end_poses.emplace_back(i * 100 + j);
		}
	}
	for (int s{ 0 }; s != start_poses.size(); ++s)
	{
		int start_pos{ start_poses[s] };
		search_queue to_search;

		to_search.push(std::pair<int, int>{ start_pos / 100, start_pos % 100 });
		while (!to_search.empty())
		{
			search_all_pt2(start_pos, data, to_search, paths);
		}
	}

	return paths.size();
}

void search_all_pt2(int start_pos, data_type& data, search_queue& to_search, positions& paths)
{
	std::pair<int, int> search_pos{ to_search.front() };
	if (data[search_pos.first][search_pos.second] == '9')
	{
		int path = start_pos * 10000 + search_pos.first * 100 + search_pos.second;
		paths.emplace_back(path);
		to_search.pop();
		return;
	}
	to_search.pop();

	if (search_pos.first < data.size() - 1 && data[search_pos.first + 1][search_pos.second] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first + 1, search_pos.second });
	if (search_pos.first > 0 && data[search_pos.first - 1][search_pos.second] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first - 1, search_pos.second });
	if (search_pos.second < data[0].size() - 1 && data[search_pos.first][search_pos.second + 1] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first, search_pos.second + 1 });
	if (search_pos.second > 0 && data[search_pos.first][search_pos.second - 1] == data[search_pos.first][search_pos.second] + 1)
		to_search.push(std::pair<int, int>{ search_pos.first, search_pos.second - 1 });
}