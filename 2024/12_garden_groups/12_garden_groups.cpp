#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::string>& data);
int part_one(std::vector<std::string>& data);
int part_two(std::vector<std::string>& data);

int main()
{
	std::vector<std::string> data;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 201.00µs  : data in + parsing
	// 3.4106ms  : part1
	// 7.0811ms  : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input day12");

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

void read_data(std::vector<std::string>& data)
{
	std::ifstream input(__INPUT_PATH);
	std::string temp;
	while (std::getline(input, temp))
		data.emplace_back(temp);

}

std::pair<int, int> get_next_pos(std::vector<std::vector<bool>> visited, std::pair<int, int> last_pos)
{
	int init_x = last_pos.second;
	for (int i{ last_pos.first }; i != visited.size(); ++i)
	{
		for (int j{ init_x }; j != visited[0].size(); ++j)
		{
			if (visited[i][j] == false) return std::pair{ i, j };
		}
		init_x = 0;
	}
	return std::pair{ visited.size(), visited[0].size() };
}

int part_one(std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<std::vector<bool>> visited{ std::vector<std::vector<bool>>{ data.size(), std::vector<bool>(data[0].size(), false) } };
	std::pair<int, int> pos{ 0, 0 };
	while (pos.first != data.size() && pos.second != data[0].size())
	{
		int plot_size{ 0 }, fence_size{ 0 };
		std::vector<std::pair<int, int>> plot;
		char ch_to_plot{ data[pos.first][pos.second] };
		plot.emplace_back(pos);
		for (int i{ 0 }; i != plot.size(); ++i)
		{
			std::pair<int, int>search_pos{ plot[i] };
			if (visited[search_pos.first][search_pos.second]) continue;
			if (search_pos.first != data.size() - 1 && data[search_pos.first + 1][search_pos.second] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first + 1, search_pos.second });
			if (search_pos.first != 0 && data[search_pos.first - 1][search_pos.second] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first - 1, search_pos.second });
			if (search_pos.second != data[0].size() - 1 && data[search_pos.first][search_pos.second + 1] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first, search_pos.second + 1 });
			if (search_pos.second != 0 && data[search_pos.first][search_pos.second - 1] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first, search_pos.second - 1 });

			if (search_pos.first == 0)
			{
				++fence_size;
				if (data[search_pos.first + 1][search_pos.second] != ch_to_plot) ++fence_size;
			}
			else if (search_pos.first == data.size() - 1)
			{
				++fence_size;
				if (data[search_pos.first - 1][search_pos.second] != ch_to_plot) ++fence_size;
			}
			else
			{
				if (data[search_pos.first + 1][search_pos.second] != ch_to_plot) ++fence_size;
				if (data[search_pos.first - 1][search_pos.second] != ch_to_plot) ++fence_size;
			}

			if (search_pos.second == 0)
			{
				++fence_size;
				if (data[search_pos.first][search_pos.second + 1] != ch_to_plot) ++fence_size;
			}
			else if (search_pos.second == data[0].size() - 1)
			{
				++fence_size;
				if (data[search_pos.first][search_pos.second - 1] != ch_to_plot) ++fence_size;
			}
			else
			{
				if (data[search_pos.first][search_pos.second + 1] != ch_to_plot) ++fence_size;
				if (data[search_pos.first][search_pos.second - 1] != ch_to_plot) ++fence_size;
			}

			visited[search_pos.first][search_pos.second] = true;
			++plot_size;
		}
		result += plot_size * fence_size;
		pos = get_next_pos(visited, pos);
	}

	return result;
}

void print_plot(std::vector<std::vector<bool>>& check_sides)
{
	for (auto da : check_sides)
	{
		for (auto ch : da)
		{
			if (ch) std::cout << 1;
			else std::cout << '.';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

int part_two(std::vector<std::string>& data)
{
	int result{ 0 };
	std::vector<std::vector<bool>> visited{ std::vector<std::vector<bool>>{ data.size(), std::vector<bool>(data[0].size(), false) } };
	std::pair<int, int> pos{ 0, 0 };
	while (pos.first != data.size() && pos.second != data[0].size())
	{
		std::vector<std::vector<bool>> check_sides{ std::vector<std::vector<bool>>{ data.size(), std::vector<bool>(data[0].size(), false) } };
		int plot_size{ 0 }, fence_size{ 0 };
		std::vector<std::pair<int, int>> plot;
		char ch_to_plot{ data[pos.first][pos.second] };
		//std::cout << "pos :" << pos.first << ", " << pos.second << '\n';
		plot.emplace_back(pos);
		int x_min{ INT32_MAX }, x_max{ 0 }, y_min{ INT32_MAX }, y_max{ 0 };
		for (int i{ 0 }; i != plot.size(); ++i)
		{
			std::pair<int, int>search_pos{ plot[i] };
			if (search_pos.first > x_max) x_max = search_pos.first;
			if (search_pos.first < x_min) x_min = search_pos.first;
			if (search_pos.second > y_max) y_max = search_pos.second;
			if (search_pos.second < y_min) y_min = search_pos.second;
			if (visited[search_pos.first][search_pos.second]) continue;
			if (search_pos.first != data.size() - 1 && data[search_pos.first + 1][search_pos.second] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first + 1, search_pos.second });
			if (search_pos.first != 0 && data[search_pos.first - 1][search_pos.second] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first - 1, search_pos.second });
			if (search_pos.second != data[0].size() - 1 && data[search_pos.first][search_pos.second + 1] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first, search_pos.second + 1 });
			if (search_pos.second != 0 && data[search_pos.first][search_pos.second - 1] == ch_to_plot)
				plot.emplace_back(std::pair{ search_pos.first, search_pos.second - 1 });

			visited[search_pos.first][search_pos.second] = true;
			check_sides[search_pos.first][search_pos.second] = true;
			++plot_size;
		}
		int fences = 0;
		bool on_fence_top = false, on_fence_bot = false;
		for (int i{ y_min }; i != y_max + 1; i++)
		{

			if (check_sides[x_min][i] && !on_fence_top)
			{
				++fences;
				on_fence_top = true;
			}
			if (!check_sides[x_min][i] && on_fence_top)
			{
				on_fence_top = false;
			}
			if (check_sides[x_max][i] && !on_fence_bot)
			{
				++fences;
				on_fence_bot = true;
			}
			if (!check_sides[x_max][i] && on_fence_bot)
			{
				on_fence_bot = false;
			}
		}
		on_fence_top = false, on_fence_bot = false;
		for (int i{ x_min }; i != x_max + 1; i++)
		{

			if (check_sides[i][y_min] && !on_fence_top)
			{
				++fences;
				on_fence_top = true;
			}
			if (!check_sides[i][y_min] && on_fence_top)
			{
				on_fence_top = false;
			}
			if (check_sides[i][y_max] && !on_fence_bot)
			{
				++fences;
				on_fence_bot = true;
			}
			if (!check_sides[i][y_max] && on_fence_bot)
			{
				on_fence_bot = false;
			}
		}
		for (int i{ x_min + 1 }; i != x_max + 1; i++)
		{
			bool on_fence_true = false, on_fence_false = false;
			for (int j{ y_min }; j != y_max + 1; j++)
			{
				if (check_sides[i][j] && !check_sides[i - 1][j] && !on_fence_true)
				{
					++fences;
					on_fence_true = true;
					on_fence_false = false;
				}
				else if (!check_sides[i][j] && check_sides[i - 1][j] && !on_fence_false)
				{
					++fences;
					on_fence_true = false;
					on_fence_false = true;
				}
				else if(check_sides[i][j] == check_sides[i - 1][j])
				{
					on_fence_true = false;
					on_fence_false = false;
				}
			}
		}
		for (int j{ y_min + 1 }; j != y_max + 1; j++)
		{
			bool on_fence_true = false, on_fence_false = false;
			for (int i{ x_min }; i != x_max + 1; i++)
			{
				if (check_sides[i][j] && !check_sides[i][j - 1] && !on_fence_true)
				{
					++fences;
					on_fence_true = true;
					on_fence_false = false;
				}
				else if (!check_sides[i][j] && check_sides[i][j - 1] && !on_fence_false)
				{
					++fences;
					on_fence_true = false;
					on_fence_false = true;
				}
				else if (check_sides[i][j] == check_sides[i][j - 1])
				{
					on_fence_true = false;
					on_fence_false = false;
				}
			}
		}
		//std::cout << "fences:" << fences << '\n';
		//print_plot(check_sides);

		result += plot_size * fences;
		pos = get_next_pos(visited, pos);
	}

	return result;
}