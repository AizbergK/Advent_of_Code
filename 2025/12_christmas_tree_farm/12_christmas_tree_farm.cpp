#include "../../other/include_everything.h"
import std;

typedef std::pair< std::array<std::pair<std::uint16_t, std::string>, 6>, std::vector<std::vector<std::uint16_t>>> data_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);

int main()
{
	// 2.3519ms  : input input day12
	// 6.4000µs : part1
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day12");
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

	std::string gift1{  };
	std::string gift2{  };
	std::string gift3{  };
	for (int i{ 0 }; i < 6; ++i)
	{
		std::getline(input, gift1);
		std::getline(input, gift1);
		std::getline(input, gift2);
		std::getline(input, gift3);
		data.first[i].second = gift1 + gift2 + gift3;
		data.first[i].first = std::count(data.first[i].second.begin(), data.first[i].second.end(), '#');
		std::getline(input, gift1);
	}

	int number; char ch;
	while (getline(input, gift1))
	{
		std::stringstream ss{ gift1 };
		data.second.emplace_back();
		ss >> number >> ch;
		data.second.back().emplace_back(number);
		ss >> number >> ch;
		data.second.back().emplace_back(number);
		while (ss >> number)
		{
			data.second.back().emplace_back(number);
		}
	}
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };
	std::uint64_t maybe { 0 };
	std::uint64_t bad   { 0 };

	for (auto& row : data.second)
	{
		int const area{ row[0] * row[1] };
		int const min_area{ 
			row[2] * data.first[0].first + 
			row[3] * data.first[1].first + 
			row[4] * data.first[2].first + 
			row[5] * data.first[3].first + 
			row[6] * data.first[4].first + 
			row[7] * data.first[5].first 
		};

		int const area_mins{ (row[0] / 3) * (row[1] / 3) };
		int const max_area{ row[2] + row[3] + row[4] + row[5] + row[6] + row[7] };

		if (min_area > area)
		{
			++bad;
			continue;
		}
		if (area_mins >= max_area)
		{
			++result;
			continue;
		}
		++maybe;
	}

	return result;
}