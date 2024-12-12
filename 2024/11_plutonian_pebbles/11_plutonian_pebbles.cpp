#include "../../other/include_everything.h"
import std;

void read_data(std::vector<long long>& data);
long long part_one(std::vector<long long> data);
long long part_two(std::vector<long long> data);
long long get_digit_count(long long number);

constexpr int steps_to_precomp_part2 = 76;
constexpr int steps_to_precomp_part1 = 26;

constexpr std::array<std::array<long long, steps_to_precomp_part1>, 10> get_step_count1()
{
	std::array<std::array<long long, steps_to_precomp_part1>, 10> precomp_steps{ std::array<std::array<long long, steps_to_precomp_part1>, 10>{ std::array<long long, steps_to_precomp_part1>{ 0, steps_to_precomp_part1 } } };
	std::array<std::vector<long long>, 10> cycle_end;
	for (int i{ 0 }; i != 10; ++i)
	{
		precomp_steps[i][0] = 1;
		precomp_steps[i][1] = 1;
		precomp_steps[i][2] = 1;
		precomp_steps[i][3] = 2;
	}
	cycle_end[0].emplace_back(1);
	for (int i{ 1 }; i != 10; ++i)
	{
		int step = 1;
		bool greater_than_ten = true;
		std::vector<long long>  numbers{ i * 2024 };
		while (greater_than_ten)
		{
			greater_than_ten = false;
			for (int j{ 0 }; j < numbers.size(); ++j)
			{
				if (numbers[j] == i)
				{
					numbers[j] = i * 2024;
					break;
					greater_than_ten = false;
				}
				if (get_digit_count(numbers[j]) % 2 == 0)
				{
					numbers.insert(numbers.begin() + j + 1, numbers[j] % (int)std::pow(10, get_digit_count(numbers[j]) / 2));
					numbers[j] /= std::pow(10, get_digit_count(numbers[j]) / 2);
					if (get_digit_count(numbers[j]) > 1) greater_than_ten = true;
					++j;
					continue;
				}
				if (!numbers[j])
				{
					numbers[j] = 1;
					continue;
				}
				numbers[j] *= 2024;
				greater_than_ten = true;

			}
			++step;
			precomp_steps[i][step] = numbers.size();
		}
		cycle_end[i] = numbers;
	}
	for (int i{ 4 }; i != 6; ++i)
	{
		precomp_steps[0][i] += precomp_steps[1][i - 1];
		for (int j{ 1 }; j != 5; j++)
		{
			for (int k{ 0 }; k != cycle_end[j].size(); k++)
			{
				precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 3];
			}

		}

	}
	for (int i{ 6 }; i != steps_to_precomp_part1; ++i)
	{
		precomp_steps[0][i] += precomp_steps[1][i - 1];
		for (int j{ 1 }; j != 10; j++)
		{
			for (int k{ 0 }; k != cycle_end[j].size(); k++)
			{
				if(j < 5)
				{
					if (cycle_end[j][k] < 10)
						precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 3];
					if (cycle_end[j][k] == 16192)
						precomp_steps[j][i] += precomp_steps[8][i - 2];
				}
				else
					if (cycle_end[j][k] < 10)
						precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 5];
				if (cycle_end[j][k] == 16192)
					precomp_steps[j][i] += precomp_steps[8][i - 4];
				
			}
		}
	}
	return precomp_steps;
}

constexpr std::array<std::array<long long, steps_to_precomp_part2>, 10> get_step_count2()
{
	std::array<std::array<long long, steps_to_precomp_part2>, 10> precomp_steps{ std::array<std::array<long long, steps_to_precomp_part2>, 10>{ std::array<long long, steps_to_precomp_part2>{ 0, steps_to_precomp_part2 } } };
	std::array<std::vector<long long>, 10> cycle_end;
	for (int i{ 0 }; i != 10; ++i)
	{
		precomp_steps[i][0] = 1;
		precomp_steps[i][1] = 1;
		precomp_steps[i][2] = 1;
		precomp_steps[i][3] = 2;
	}
	cycle_end[0].emplace_back(1);
	for (int i{ 1 }; i != 10; ++i)
	{
		int step = 1;
		bool greater_than_ten = true;
		std::vector<long long>  numbers{ i * 2024 };
		while (greater_than_ten)
		{
			greater_than_ten = false;
			for (int j{ 0 }; j < numbers.size(); ++j)
			{
				if (numbers[j] == i)
				{
					numbers[j] = i * 2024;
					break;
					greater_than_ten = false;
				}
				if (get_digit_count(numbers[j]) % 2 == 0)
				{
					numbers.insert(numbers.begin() + j + 1, numbers[j] % (int)std::pow(10, get_digit_count(numbers[j]) / 2));
					numbers[j] /= std::pow(10, get_digit_count(numbers[j]) / 2);
					if (get_digit_count(numbers[j]) > 1) greater_than_ten = true;
					++j;
					continue;
				}
				if (!numbers[j])
				{
					numbers[j] = 1;
					continue;
				}
				numbers[j] *= 2024;
				greater_than_ten = true;

			}
			++step;
			precomp_steps[i][step] = numbers.size();
		}
		cycle_end[i] = numbers;
	}
	for (int i{ 4 }; i != 6; ++i)
	{
		precomp_steps[0][i] += precomp_steps[1][i - 1];
		for (int j{ 1 }; j != 5; j++)
		{
			for (int k{ 0 }; k != cycle_end[j].size(); k++)
			{
				precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 3];
			}

		}

	}
	for (int i{ 6 }; i != steps_to_precomp_part2; ++i)
	{
		precomp_steps[0][i] += precomp_steps[1][i - 1];
		for (int j{ 1 }; j != 10; j++)
		{
			for (int k{ 0 }; k != cycle_end[j].size(); k++)
			{
				if (j < 5)
				{
					if (cycle_end[j][k] < 10)
						precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 3];
					if (cycle_end[j][k] == 16192)
						precomp_steps[j][i] += precomp_steps[8][i - 2];
				}
				else
					if (cycle_end[j][k] < 10)
						precomp_steps[j][i] += precomp_steps[cycle_end[j][k]][i - 5];
				if (cycle_end[j][k] == 16192)
					precomp_steps[j][i] += precomp_steps[8][i - 4];

			}
		}
	}
	return precomp_steps;
}

const std::array<std::array<long long, steps_to_precomp_part1>, 10> precomp_steps1{ get_step_count1() };
const std::array<std::array<long long, steps_to_precomp_part2>, 10> precomp_steps2{ get_step_count2() };
int main()
{
	std::vector<long long> data;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 204.70µs  : data in + parsing
	// 141.30µs  : part1
	// 214.60s   : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input day11");

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

void read_data(std::vector<long long>& data)
{
	std::ifstream input(__INPUT_PATH);
	int temp;
	while (input >> temp)
	{
		data.emplace_back(temp);
	}
}

long long part_one(std::vector<long long> data)
{
	long long result = 0, step = 0;

	while (data.size() && step != steps_to_precomp_part1 - 1)
	{
		for (std::uint64_t i{ 0 }, end{ data.size() }; i != end; ++i)
		{
			if (data[i] < 10)
			{
				result += precomp_steps1[data[i]][steps_to_precomp_part1 - step - 1];
				data.erase(data.begin() + i);
				--i;
				--end;
				continue;
			}
			if (get_digit_count(data[i]) % 2 == 0)
			{
				data.emplace_back(data[i] % (int)std::pow(10, get_digit_count(data[i]) / 2));
				data[i] /= std::pow(10, get_digit_count(data[i]) / 2);
				continue;
			}
			data[i] *= 2024;
		}
		++step;
	}
	result += data.size();
	return result;
}

long long get_digit_count(long long number)
{
	long long count = 0;
	do
	{
		++count;
		number /= 10;
	} 
	while (number);

	return count;
}

long long part_two(std::vector<long long> data)
{
	long long result = 0, step = 0;

	while (data.size() && step != steps_to_precomp_part2 - 1)
	{
		for (std::uint64_t i{ 0 }, end{ data.size() }; i != end; ++i)
		{
			if (data[i] < 10)
			{
				result += precomp_steps2[data[i]][steps_to_precomp_part2 - step - 1];
				data.erase(data.begin() + i);
				--i;
				--end;
				continue;
			}
			if (get_digit_count(data[i]) % 2 == 0)
			{
				data.emplace_back(data[i] % (int)std::pow(10, get_digit_count(data[i]) / 2));
				data[i] /= std::pow(10, get_digit_count(data[i]) / 2);
				continue;
			}
			data[i] *= 2024;
		}
		++step;
	}
	result += data.size();
	return result;
}