#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas);
int part_one(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas);
int part_two(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas);
void get_antennas(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas);
bool in_bounds(std::pair<int, int> pos, int x_max, int y_max);

int main()
{
	std::vector<std::string> data;
	std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 138.00µs  : data in + parsing
	// 10.400µs  : part1
	// 10.000µs  : part2
	input_timer.startTimer();
	read_data(data, antennas);
	input_timer.getDuration(u8"input day8");

	part1_timer.startTimer();
	part_one_result = part_one(data, antennas);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data, antennas);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas)
{
	std::ifstream input(__INPUT_PATH);
	std::string temp;
	while (std::getline(input, temp))
		data.emplace_back(temp);
	get_antennas(data, antennas);
}

int part_one(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas)
{
	int result = 0;
	std::vector<std::string> antinodes{ std::vector<std::string>{data.size(), std::string(data[0].size(), '.')} };
	for (auto it = antennas.begin(); it != antennas.end(); ++it)
	{
		for (int i{ 0 }; i != it->second.size() - 1; ++i)
		{
			for (int j{ i + 1 }; j != it->second.size(); ++j)
			{
				std::pair<int, int> distance{ it->second[i].first - it->second[j].first, it->second[i].second - it->second[j].second };
				std::pair<int, int> antinode1{ it->second[i].first + distance.first, it->second[i].second + distance.second };
				std::pair<int, int> antinode2{ it->second[j].first - distance.first, it->second[j].second - distance.second };
				if(in_bounds(antinode1, data.size(), data[0].size()))
					antinodes[antinode1.first][antinode1.second] = '#';
				if(in_bounds(antinode2, data.size(), data[0].size()))
					antinodes[antinode2.first][antinode2.second] = '#';
			}
		}
	}
	for (auto row : antinodes)
		for (auto ch : row)
			if (ch == '#') ++result;



	return result;
}



int part_two(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas)
{
	int result = 0;
	std::vector<std::string> antinodes{ std::vector<std::string>{data.size(), std::string(data[0].size(), '.')} };
	for (auto it = antennas.begin(); it != antennas.end(); ++it)
	{
		for (int i{ 0 }; i != it->second.size() - 1; ++i)
		{
			antinodes[it->second[i].first][it->second[i].second] = '#';
			for (int j{ i + 1 }; j != it->second.size(); ++j)
			{
				std::pair<int, int> distance{ it->second[i].first - it->second[j].first, it->second[i].second - it->second[j].second };
				std::pair<int, int> antinode1{ it->second[i].first + distance.first, it->second[i].second + distance.second };
				std::pair<int, int> antinode2{ it->second[j].first - distance.first, it->second[j].second - distance.second };
				while (in_bounds(antinode1, data.size(), data[0].size()))
				{
					antinodes[antinode1.first][antinode1.second] = '#';
					antinode1.first += distance.first;
					antinode1.second += distance.second;
				}
				while (in_bounds(antinode2, data.size(), data[0].size()))
				{
					antinodes[antinode2.first][antinode2.second] = '#';
					antinode2.first -= distance.first;
					antinode2.second -= distance.second;
				}
			}
		}
		antinodes[it->second[it->second.size() - 1].first][it->second[it->second.size() - 1].second] = '#';
	}
	for (auto row : antinodes)
		for (auto ch : row)
			if (ch == '#') ++result;

	return result;
}

bool in_bounds(std::pair<int, int> pos, int x_max, int y_max)
{
	if (pos.first < 0 || pos.first >= x_max || pos.second < 0 || pos.second >= y_max) return false;

	return true;
}

void get_antennas(std::vector<std::string>& data, std::unordered_map<char, std::vector<std::pair<int, int>>>& antennas)
{
	for (int i{ 0 }; i != data.size(); ++i)
	{
		for (int j{ 0 }; j != data[i].size(); ++j)
		{
			if (data[i][j] != '.')
				antennas[data[i][j]].emplace_back(std::pair{ i, j });
		}
	}
}