#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<long long>& numbers);
long long part_one(std::vector<long long>& numbers, std::vector < std::vector<int>>& price);
int part_two(std::vector<std::vector<int>>& price);

int main()
{
	// 943.60µs  : data in + parsing
	// 17.094ms  : part1
	// 6.6978s   : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day22");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::vector<long long> numbers;
	std::vector<std::vector<int>> price;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, numbers);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(numbers, price);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(price);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<long long>& numbers)
{
	std::ifstream input(input_path);
	long long temp;
	while (input >> temp)
		numbers.emplace_back(temp);
}

long long part_one(std::vector<long long>& numbers, std::vector < std::vector<int>>& price)
{
	
	const long long prune = 16777216 - 1;
	long long result{ 0 };
	for (auto number : numbers)
	{
		price.emplace_back(std::vector<int>{});
		for (int i{ 0 }; i != 2000; ++i)
		{
			number = (number ^ (number << 6)) & prune;
			number = (number ^ (number >> 5)) & prune;
			number = (number ^ (number << 11)) & prune;
			price[price.size() - 1].emplace_back(number % 10);
		}
		result += number;
	}

	return result;
}

int part_two(std::vector<std::vector<int>>& price)
{
	int result{ 0 };
	std::vector<std::vector<int>> dif;
	std::vector<int> sums_for_sequences;
	for (auto& row : price)
	{
		dif.emplace_back(std::vector<int>{});
		for (int i{ 1 }; i != row.size(); ++i)
			dif[dif.size() - 1].emplace_back(row[i] - row[i - 1]);
	}
	for (int i{ 0 }; i != dif[0].size() - 4; ++i)
	{
		auto it_start{ dif[0].begin() + i}, it_end{dif[0].begin() + i + 4};
		int sequence_price{ 0 };
		for (int j{ 0 }; j != dif.size(); ++j)
		{
			auto location{ std::search(dif[j].begin(), dif[j].end(), it_start, it_end) };
			if (location != dif[j].end())
				sequence_price += price[j][location - dif[j].begin() + 4];
		}
		sums_for_sequences.emplace_back(sequence_price);
	}

	result = *std::max_element(sums_for_sequences.begin(), sums_for_sequences.end());
	return result;
}