#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::pair<std::string, std::string>>& data);
std::uint64_t part_one(std::vector<std::pair<std::string, std::string>>& data);
std::uint64_t part_two(std::vector<std::pair<std::string, std::string>>& data);

int main()
{
	// 67.700µs : input input day02
	// 27.800µs : part1
	// 30.602ms : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day02");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	std::uint64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	std::vector<std::pair<std::string, std::string>> data;

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

void read_data(std::string& input_path, std::vector<std::pair<std::string, std::string>>& data)
{
	std::ifstream input(input_path);

	std::string first_temp;
	std::string second_temp;

	while (std::getline(input, first_temp, '-'))
	{
		std::getline(input, second_temp, ',');
		data.emplace_back(first_temp, second_temp);
	}

}

std::uint64_t part_one(std::vector<std::pair<std::string, std::string>>& data)
{
	std::uint64_t result{ 0 };

	for (const auto& [first, second] : data)
	{
		const bool first_is_even{ first.size() % 2 == 0 };
		const bool second_is_even{ second.size() % 2 == 0 };

		std::uint64_t start{ 0 };
		std::uint64_t end{ 0 };

		if (first_is_even)
		{
			const std::uint64_t first_half{ std::stoull(first.substr(0, first.length() / 2)) };
			const std::uint64_t second_half{ std::stoull(first.substr(first.length() / 2)) };
			if (first_half >= second_half)
			{
				start = first_half;
			}
			else
			{
				start = first_half + 1;
			}
		}
		else
		{
			start = std::pow(10, first.length() / 2);
		}

		if (second_is_even)
		{
			const std::uint64_t first_half{ std::stoull(second.substr(0, second.length() / 2)) };
			const std::uint64_t second_half{ std::stoull(second.substr(second.length() / 2)) };
			if (first_half <= second_half)
			{
				end = first_half;
			}
			else
			{
				end = first_half - 1;
			}
		}
		else
		{
			end = 9;
			for (int i{ 0 }; i < (second.length() / 2) - 1; ++i)
			{
				end *= 10;
				end += 9;
			}
		}
		auto new_start{ start };
		while(start <= end)
		{
			std::string half{ std::to_string(start) };
			half += half;
			result += std::stoull(half);
			++start;
		}

		if (new_start > 0)
		{
			new_start = new_start;
		}
	}

	return result;
}

std::uint64_t part_two(std::vector<std::pair<std::string, std::string>>& data)
{
	std::uint64_t result{ 0 };

	for (const auto& [first, second] : data)
	{
		const bool first_is_even{ first.size() % 2 == 0 };
		const bool second_is_even{ second.size() % 2 == 0 };

		std::uint64_t start{ std::stoull(first) };
		std::uint64_t end{ std::stoull(second) };

		for(std::uint64_t num{ start }; num <= end; ++num)
		{
			bool is_invalid{ false };
			std::string str_to_check{ std::to_string(num) };
			for (int i{ 2 }; i <= str_to_check.size(); ++i)
			{
				if (str_to_check.size() % i != 0) continue;
				std::uint64_t step{ str_to_check.size() / i };
				for (std::uint64_t j{ step }; j < str_to_check.size(); j += step)
				{
					std::string_view l{ str_to_check.data() + j - step, step };
					std::string_view r{ str_to_check.data() + j, step };
					if (l != r)
					{
						break;
					}
					if (j + step == str_to_check.size())
					{
						is_invalid = true;
						break;
					}
				}

				if (is_invalid) break;
			}
			if (is_invalid)
			{
				result += num;
			}

			++start;
		}
	}

	return result;
}