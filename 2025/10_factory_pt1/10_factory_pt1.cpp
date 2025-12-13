#include "../../other/include_everything.h"
import std;

typedef std::pair<std::vector<std::vector<std::bitset<32>>>, std::vector<std::vector<int>>> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 268.80µs : input input day10
	// 646.00µs : part1
	// python   : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day10");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::uint64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	data_type data;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	std::println("{0}", part_one_result);
	std::println();
}

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);

	std::string line;
	while (std::getline(input, line))
	{
		data.first.emplace_back();
		data.second.emplace_back();
		int i{ 0 };
		std::bitset<32> temp{ 0 };
		for (; line[i] != ']'; ++i)
		{
			if (line[i] == '#')
			{
				temp.set(i - 1);
			}
		}
		data.first.back().emplace_back(temp);
		temp.reset();
		i += 2;

		while (line[i] == '(')
		{
			while (line[i] != ')')
			{
				temp.set(line[i + 1] - '0');
				i += 2;
			}

			data.first.back().emplace_back(temp);
			temp.reset();
			i += 2;
		}

		int num;
		char const* ptr{ line.data() + i + 1 };
		char const* const end{ line.data() + line.size() };
		while (ptr != end)
		{
			auto const res{ std::from_chars(ptr, end, num) };
			ptr = res.ptr + 1;
			data.second.back().emplace_back(num);
		}
	}
}

inline int next_combination(int x) //Gosper's hack
{
	int c = x & -x;
	int r = x + c;
	return (((r ^ x) >> 2) / c) | r;
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };

	for (int i{ 0 }; i < data.first.size(); ++i)
	{
		int combinations{ 0 };
		int const no_btns{ static_cast<int>(data.first[i].size()) };
		int max_val{ 1 << no_btns - 1 };
		for (int k{ 1 }; k <= data.first[i].size() - 1; ++k)
		{
			combinations = 1;
			for (int j{ 1 }; j < k; ++j)
			{
				combinations <<= 1;
				combinations |= 1;
			}
			bool found_one{ false };
			while (combinations < max_val)
			{
				std::bitset<32> combination_result{ 0 };
				std::bitset<32> combination{ static_cast<std::uint64_t>(combinations) };
				for (int j{ 0 }; j < no_btns; ++j)
				{
					if (combination[j])
					{
						combination_result ^= data.first[i][j + 1];
					}
				}
				if (combination_result == data.first[i][0])
				{
					found_one = true;
					break;
				}
				combinations = next_combination(combinations);
			}
			if (found_one)
			{
				result += k;
				break;
			}
		}
	}

	return result;
}