#include "../../other/include_everything.h"
import std;

typedef std::pair<std::unordered_map<int,std::vector<int>>, std::unordered_map<int, std::vector<int>>> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::int64_t part_one(data_type& data);
std::int64_t part_two(data_type& data);

int main()
{
	// 000000ms  : data in + parsing
	// 000000ms  : part1
	// 000000ms  : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day11");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	std::int64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	data_type data;

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

inline int ps(char a, char b, char c) 
{ 
	return ((int)a << 16) + ((int)b << 8) + (int)c; 
}
inline int ps(const char* str) 
{ 
	return ((int)str[0] << 16) + ((int)str[1] << 8) + (int)str[2]; 
}

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);

	std::string temp;
	while (std::getline(input, temp))
	{
		int const first_node{ ps(temp[0],temp[1], temp[2]) };
		data.first[first_node];

		for (int i{ 5 }; i < temp.size(); i += 4)
		{
			int const next_node{ ps(temp[i], temp[i + 1], temp[i + 2]) };
			data.first[first_node].emplace_back(next_node);
			data.second[next_node].emplace_back(first_node);
		}
	}
}

std::int64_t paths_from_to(data_type& const data, int const from, int const to)
{
	std::unordered_map<int, std::int64_t> node_counts;
	std::stack<int> next_nodes{  };
	for (auto node : data.first[from])
	{
		next_nodes.emplace(node);
	}

	for (auto node : data.first[from])
	{
		for (auto back_node : data.second[node])
		{
			node_counts[back_node] = -1;
		}
	}
	node_counts[from] = 1;

	while (node_counts[to] == 0 && !next_nodes.empty())
	{
		if (node_counts[next_nodes.top()] != 0)
		{
			next_nodes.pop();
			continue;
		}

		bool has_zero_input{ false };
		for (auto const node : data.second[next_nodes.top()])
		{
			if (node_counts[node] == 0)
			{
				has_zero_input = true;
				next_nodes.emplace(node);
				break;
			}
		}
		if (!has_zero_input)
		{
			auto const finished_node{ next_nodes.top() };
			next_nodes.pop();

			bool is_non_zero{ false };
			for (auto const node : data.second[finished_node])
			{
				if (node_counts[node] != -1)
				{
					is_non_zero = true;
				}
				if (node_counts[node] > 0)
				{
					node_counts[finished_node] += node_counts[node];
				}
			}
			if (!is_non_zero)
			{
				node_counts[finished_node] = -1;
			}
			else
			{
				for (auto const node : data.first[finished_node])
				{
					next_nodes.emplace(node);
				}
			}
		}
	}

	return node_counts[to] < 0 ? 0 : node_counts[to];
}

std::int64_t part_one(data_type& data)
{
	return paths_from_to(data, ps("you"), ps("out"));
}

std::int64_t part_two(data_type& data)
{
	std::int64_t const svr_fft{ paths_from_to(data, ps("svr"), ps("fft")) };
	std::int64_t const svr_dac{ paths_from_to(data, ps("svr"), ps("dac")) };

	std::int64_t const dac_fft{ paths_from_to(data, ps("dac"), ps("fft")) };
	std::int64_t const fft_dac{ paths_from_to(data, ps("fft"), ps("dac")) };

	std::int64_t const dac_out{ paths_from_to(data, ps("dac"), ps("out")) };
	std::int64_t const fft_out{ paths_from_to(data, ps("fft"), ps("out")) };

	std::int64_t const result{ svr_fft * fft_dac * dac_out + svr_dac * dac_fft * fft_out };

	return result;
}