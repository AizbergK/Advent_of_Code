#include "../../other/include_everything.h"
import std;

typedef std::vector<std::string> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 144.10µs : input input day06
	// 45.700µs : part1
	// 171.70µs : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day06");
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

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);

	std::string line{  };
	while (getline(input, line))
	{
		data.emplace_back(line);
	}
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };

	std::vector<char> vec_ops{  };
	for (char c : data.back())
	{
		if (c != ' ') {
			vec_ops.push_back(c == '+');
		}
	}

	std::vector<std::uint64_t> vec_acc{  };
	std::vector<std::uint64_t> vec_tmp{  };
	vec_acc.resize(vec_ops.size());
	vec_tmp.resize(vec_ops.size());

	const char* ptr{ data[0].data() };
	const char const* end{ ptr + data[0].size() };
	for (int i = 0; ptr < end; ptr++) 
	{
		if (*ptr != ' ')
		{
			auto res{ std::from_chars(ptr, end, vec_acc[i]) };
			ptr = res.ptr;
			++i;
		}
	}

	for(int i{ 1 }; i < data.size() - 1; ++i)
	{
		const char* ptr{ data[i].data() };
		const char const* end{ ptr + data[i].size() };
		for (int i = 0; ptr < end; ptr++)
		{
			if (*ptr != ' ')
			{
				auto res = std::from_chars(ptr, end, vec_tmp[i]);
				ptr = res.ptr;
				++i;
			}
		}

		for (int j{ 0 }; j < vec_acc.size(); ++j)
		{
			if (vec_ops[j])
			{
				vec_acc[j] += vec_tmp[j];
			}
			else
			{
				vec_acc[j] *= vec_tmp[j];
			}
		}
	}

	result = std::accumulate(vec_acc.begin(), vec_acc.end(), 0ULL);

	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 0 };

	for (int i{ 1 }; i < data.back().size(); ++i)
	{
		if (data.back()[i] == ' ') data.back()[i] = data.back()[i - 1];
	}

	std::vector<char> digits{  };
	digits.resize(data.size());

	std::uint64_t line_result{ 0 };
	for(int i{ 0 }; i < data[0].size(); ++i)
	{
		std::uint32_t number{ 0 };
		
		for (int j{ 0 }; j < digits.size(); ++j)
		{
			digits[j] = data[j][i];
		}
		for (auto ch : digits)
		{
			if (std::isdigit(ch))
			{
				number *= 10;
				number += ch - '0';
			}
		}
		if (number == 0)
		{
			result += line_result;
			line_result = 0;
			continue;
		}
		if (line_result == 0)			line_result = number;
		else if (digits.back() == '+')	line_result += number;
		else							line_result *= number;
	}
	result += line_result;

	return result;
}