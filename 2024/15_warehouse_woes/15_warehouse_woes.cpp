#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string input, std::vector<std::string>& grid, std::string &movements, std::pair<int, int>& start_pos);
int part_one(std::vector<std::string> grid, std::string& movements, std::pair<int, int> start_pos);
long long part_two(std::vector<std::string> grid, std::string& movements, std::pair<int, int> start_pos);

int main()
{
	// 179.40µs  : data in + parsing
	// 221.70µs  : part1
	// 581.70µs  : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day15");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::vector<std::string> grid;
	std::string movements;
	std::pair<int, int> start_pos;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, grid, movements, start_pos);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(grid, movements, start_pos);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(grid, movements, start_pos);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string input_path, std::vector<std::string>& grid, std::string& movements, std::pair<int, int>& start_pos)
{
	std::ifstream input(input_path);
	std::string temp{ "a" };
	while (std::getline(input, temp) && temp != "")
	{
		grid.emplace_back(temp);
	}
	while (std::getline(input, temp))
	{
		movements += temp;
	}
	for (int i{ 0 }; i != grid[0].size(); ++i)
	{
		for (int j{ 0 }; j != grid.size(); ++j)
		{
			if (grid[i][j] == '@')
			{
				start_pos.first = i;
				start_pos.second = j;
				return;
			}
		}
	}
}

void print_grid(std::vector<std::string>& grid)
{
	for (auto a : grid)
	{
		for (auto b : a)
		{
			std::cout << b;
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

void move(std::pair<int, int> direction, std::vector<std::string>& grid, std::pair<int, int>& cur_pos)
{
	int i{ cur_pos.first }, j{ cur_pos.second };
	while (grid[i][j] != '#')
	{
		if (grid[i][j] == '.')
		{
			int t{ i }, u{ j };
			while (t != cur_pos.first || u != cur_pos.second)
			{
				grid[t][u] = grid[t - direction.first][u - direction.second];
				t -= direction.first;
				u -= direction.second;
			}
			grid[t][u] = '.';
			cur_pos.first += direction.first;
			cur_pos.second += direction.second;

			return;
		}
		i += direction.first;
		j += direction.second;
	}
}

int part_one(std::vector<std::string> grid, std::string& movements, std::pair<int, int> start_pos)
{
	int result{ 0 };

	for (auto ch : movements)
	{
		switch (ch)
		{
		case '^':
			move(std::pair<int, int>{-1, 0}, grid, start_pos);
			break;
		case '<':
			move(std::pair<int, int>{0, -1}, grid, start_pos);
			break;
		case '>':
			move(std::pair<int, int>{0, 1}, grid, start_pos);
			break;
		case 'v':
			move(std::pair<int, int>{1, 0}, grid, start_pos);
			break;
		}
	}
	for (int i{ 1 }; i != grid[0].size() - 1; ++i)
	{
		for (int j{ 1 }; j != grid.size() - 1; ++j)
		{
			if (grid[i][j] == 'O')
				result += 100 * i + j;
		}
	}
	return result;
}

void move_part2(std::pair<int, int> direction, std::vector<std::string>& grid, std::pair<int, int>& cur_pos)
{
	int i{ cur_pos.first + direction.first }, j{ cur_pos.second + direction.second };
	if(direction.second)
	{
		while (grid[i][j] != '#')
		{
			if (grid[i][j] == '.')
			{
				int u{ j };
				while (u != cur_pos.second)
				{
					grid[i][u] = grid[i][u - direction.second];
					u -= direction.second;
				}
				grid[i][u] = '.';
				cur_pos.first += direction.first;
				cur_pos.second += direction.second;

				return;
			}
			i += direction.first;
			j += direction.second;
		}
	}
	else
	{
		if (grid[i][j] == '#') return;
		if (grid[i][j] == '.')
		{
			grid[i][j] = '@';
			grid[cur_pos.first][cur_pos.second] = '.';
			cur_pos.first += direction.first;
			cur_pos.second += direction.second;
			return;
		}
		std::vector<std::pair<int, int>> boxes{};
		if (grid[i][j] == '[')
		{
			boxes.emplace_back(std::pair<int, int>{i, j});
			boxes.emplace_back(std::pair<int, int>{i, j + 1});
		}
		if (grid[i][j] == ']')
		{
			boxes.emplace_back(std::pair<int, int>{i, j - 1});
			boxes.emplace_back(std::pair<int, int>{i, j});
		}
		for (int i{ 0 }; i != boxes.size(); ++i)
		{
			bool same_symb = grid[boxes[i].first + direction.first][boxes[i].second] == grid[boxes[i].first][boxes[i].second];
			if (grid[boxes[i].first + direction.first][boxes[i].second] == '#') return;
			if (same_symb)
			{
				boxes.emplace_back(std::pair<int, int>{ boxes[i].first + direction.first, boxes[i].second });
			}
			else if (grid[boxes[i].first + direction.first][boxes[i].second] == '[' && !same_symb)
			{
				if(std::find(boxes.begin(), boxes.end(), std::pair<int,int>{ boxes[i].first + direction.first, boxes[i].second }) == boxes.end())
				{
					boxes.emplace_back(std::pair<int, int>{ boxes[i].first + direction.first, boxes[i].second });
					boxes.emplace_back(std::pair<int, int>{boxes[i].first + direction.first, boxes[i].second + 1 });
				}
			}
			else if (grid[boxes[i].first + direction.first][boxes[i].second] == ']' && !same_symb)
			{
				if (std::find(boxes.begin(), boxes.end(), std::pair<int, int>{ boxes[i].first + direction.first, boxes[i].second }) == boxes.end())
				{
					boxes.emplace_back(std::pair<int, int>{boxes[i].first + direction.first, boxes[i].second });
					boxes.emplace_back(std::pair<int, int>{boxes[i].first + direction.first, boxes[i].second - 1 });
				}
			}
		}
		for (int i{ (int)boxes.size() - 1 }; i >= 0; --i)
		{
			grid[boxes[i].first + direction.first][boxes[i].second] = grid[boxes[i].first][boxes[i].second];
			grid[boxes[i].first][boxes[i].second] = '.';
		}
		grid[cur_pos.first][cur_pos.second] = '.';
		cur_pos.first += direction.first;
		grid[cur_pos.first][cur_pos.second] = '@';
	}
}

long long part_two(std::vector<std::string> grid, std::string& movements, std::pair<int, int> start_pos)
{
	start_pos.second *= 2;
	long long result{ 0 };
	for (auto& str : grid)
	{
		int width{ (int)str.size() - 1 };
		str += str;
		while (width >= 0)
		{
			if (str[width] == '#')
			{
				str[width * 2] = '#';
				str[width * 2 + 1] = '#';
			}
			if (str[width] == '@')
			{
				str[width * 2] = '@';
				str[width * 2 + 1] = '.';
			}
			if (str[width] == '.')
			{
				str[width * 2] = '.';
				str[width * 2 + 1] = '.';
			}
			if (str[width] == 'O')
			{
				str[width * 2] = '[';
				str[width * 2 + 1] = ']';
			}
			--width;
		}
	}

	for (auto ch : movements)
	{
		switch (ch)
		{
		case '^':
			move_part2(std::pair<int, int>{-1, 0}, grid, start_pos);
			break;
		case '<':
			move_part2(std::pair<int, int>{0, -1}, grid, start_pos);
			break;
		case '>':
			move_part2(std::pair<int, int>{0, 1}, grid, start_pos);
			break;
		case 'v':
			move_part2(std::pair<int, int>{1, 0}, grid, start_pos);
			break;
		}
	}

	for (int i{ 0 }; i != grid.size(); ++i)
	{
		for (int j{ 0 }; j != grid[0].size(); ++j)
		{
			if (grid[i][j] == '[')
				result += 100 * i + j;
		}
	}

	return result;
}