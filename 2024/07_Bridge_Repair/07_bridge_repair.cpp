#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::vector<std::uint64_t>>& data);
std::uint64_t part_one(std::vector<std::vector<std::uint64_t>>& data);
std::uint64_t part_two(std::vector<std::vector<std::uint64_t>>& data);
bool check_equation(std::uint64_t result, int start_index, std::vector<std::uint64_t>& equation);
std::uint64_t concat(std::uint64_t left, std::uint64_t right);
int main()
{
	std::vector<std::vector<std::uint64_t>> data;
	std::uint64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 7.5240ms  : data in + parsing
	// 621.60µs  : part1
	// 223.03ms  : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input");

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

void read_data(std::vector<std::vector<std::uint64_t>>& data)
{
	std::ifstream input(__INPUT_PATH);
	std::uint64_t temp_i;
	char temp_c;
	std::string line;
	
	while (std::getline(input, line))
	{
		std::stringstream ss(line);
		std::vector<std::uint64_t> equation;
		ss >> temp_i;
		equation.emplace_back(temp_i);
		ss >> temp_c;
		while (ss >> temp_i)
		{
			equation.emplace_back(temp_i);
		}
		data.emplace_back(equation);
	}
}

std::uint64_t part_one(std::vector<std::vector<std::uint64_t>>& data)
{
	std::uint64_t result = 0;

	for (int i{ 0 }; i != data.size(); ++i)
	{
		std::vector<std::uint64_t> results;
		if (check_equation(data[i][1], 1, data[i]))
		{
			result += data[i][0];
		}
	}

	return result;
}

bool check_equation(std::uint64_t result, int start_index, std::vector<std::uint64_t>& equation)
{
	++start_index;
	if (start_index == equation.size())
	{
		return equation[0] == result;
	}

	return check_equation(result + equation[start_index], start_index, equation) || check_equation(result * equation[start_index], start_index, equation);
}

bool check_equation_part2(std::uint64_t result, int start_index, std::vector<std::uint64_t>& equation)
{
	++start_index;
	if (start_index == equation.size())
	{
		return equation[0] == result;
	}

	return check_equation_part2(result + equation[start_index], start_index, equation)
		|| check_equation_part2(result * equation[start_index], start_index, equation)
		|| check_equation_part2(concat(result, equation[start_index]), start_index, equation);
}

std::uint64_t concat(std::uint64_t left, std::uint64_t right)
{
	std::uint64_t number = std::stoll(std::to_string(left) + std::to_string(right));
	return number;
}

std::uint64_t part_two(std::vector<std::vector<std::uint64_t>>& data)
{
	std::uint64_t result = 0;

	for (int i{ 0 }; i != data.size(); ++i)
	{
		std::vector<std::uint64_t> results;
		if (check_equation_part2(data[i][1], 1, data[i]))
		{
			result += data[i][0];
		}
	}

	return result;
}