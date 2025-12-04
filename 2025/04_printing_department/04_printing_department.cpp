#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::string>& data);
int part_one(std::vector<std::string>& data);
int part_two(std::vector<std::string>& data);

int main()
{
	// 179.10µs : input input day15
	// 212.10µs : part1
	// 3.7431ms : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day15");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{

	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	std::vector<std::string> data;

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

void read_data(std::string& input_path, std::vector<std::string>& data)
{
	std::ifstream input(input_path);

	std::string temp;
	std::getline(input, temp);
	std::string empty_string(temp.size() + 2, '.');
	data.emplace_back(empty_string); //padding
	do
	{
		data.emplace_back('.' + temp + '.');

	} while (std::getline(input, temp));
	data.emplace_back(empty_string);
}

int part_one(std::vector<std::string>& data)
{
	int result{ 0 };

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ 0 }; j < data[0].size(); ++j)
		{
			if (data[i][j] == '@')
			{
				int neighbours{ -1 };
				for (int k{ -1 }; k <= 1; ++k)
				{
					for (int l{ -1 }; l <= 1; ++l)
					{
						if (data[i + k][j + l] == '@')
						{
							++neighbours;
						}
					}
				}
				if (neighbours < 4)
				{
					++result;
				}
			}
		}
	}

	return result;
}

int part_two(std::vector<std::string>& data)
{
	int result{ 0 };

	bool found_one{ true };

	while (found_one)
	{
		found_one = false;
		for (int i{ 0 }; i < data.size(); ++i)
		{
			for (int j{ 0 }; j < data[0].size(); ++j)
			{
				if (data[i][j] == '@')
				{
					int neighbours{ -1 };
					for (int k{ -1 }; k <= 1; ++k)
					{
						for (int l{ -1 }; l <= 1; ++l)
						{
							if (data[i + k][j + l] == '@' || data[i + k][j + l] == '#')
							{
								++neighbours;
							}
						}
					}
					if (neighbours < 4)
					{
						++result;
						found_one = true;
						data[i][j] = '#';
					}
				}
			}
		}
		for (int i{ 0 }; i < data.size(); ++i)
		{
			for (int j{ 0 }; j < data[0].size(); ++j)
			{
				if (data[i][j] == '#')
				{
					data[i][j] = '.';
				}
			}
		}
	} 

	return result;
}