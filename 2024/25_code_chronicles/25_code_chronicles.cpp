#include "../../other/include_everything.h"
import std;

typedef std::vector<std::string> schematic;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<schematic>& lock_sch, std::vector<schematic>& key_sch, std::vector<std::vector<int>>& locks, std::vector<std::vector<int>>& keys);
int part_one(std::vector<schematic>& lock_sch, std::vector<schematic>& key_sch, std::vector<std::vector<int>>& locks, std::vector<std::vector<int>>& keys);
int part_two();



int main()
{
	// 000000ms  : data in + parsing
	// 000000ms  : part1
	// 000000ms  : part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day15");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::vector<schematic> lock_sch, key_sch;
	std::vector<std::vector<int>> locks, keys;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, lock_sch, key_sch, locks, keys);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(lock_sch, key_sch, locks, keys);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two();
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<schematic>& lock_sch, std::vector<schematic>& key_sch, std::vector<std::vector<int>>& locks, std::vector<std::vector<int>>& keys)
{
	std::ifstream input(input_path);
	std::string temp;
	schematic temp_sch;
	while (std::getline(input, temp))
	{
		if (temp == "")
		{
			if (temp_sch[0][0] == '#')
				lock_sch.emplace_back(temp_sch);
			else
				key_sch.emplace_back(temp_sch);
			temp_sch.clear();
		}
		else
		{
			temp_sch.emplace_back(temp);
		}
	}
	if (temp_sch[0][0] == '#')
		lock_sch.emplace_back(temp_sch);
	else
		key_sch.emplace_back(temp_sch);
	temp_sch.clear();

	for (auto& sch : key_sch)
	{
		std::vector<int> heights(5, 0);
		for (int i{ 0 }; i != sch.size(); ++i)
			for (int j{ 0 }; j != sch[0].size(); ++j)
				if (sch[i][j] == '#')
					++heights[j];
		for (auto& i : heights) --i;
		keys.emplace_back(heights);
	}
	for (auto& sch : lock_sch)
	{
		std::vector<int> heights(5, 0);
		for (int i{ 0 }; i != sch.size(); ++i)
			for (int j{ 0 }; j != sch[0].size(); ++j)
				if (sch[i][j] == '#')
					++heights[j];
		for (auto& i : heights) --i;
		locks.emplace_back(heights);
	}
}

int part_one(std::vector<schematic>& lock_sch, std::vector<schematic>& key_sch, std::vector<std::vector<int>>& locks, std::vector<std::vector<int>>& keys)
{
	int result{ 0 };

	for(auto& lock : locks)
		for(auto& key : keys)
		{
			bool good_key{ true };
			for (int i{ 0 }; i != lock.size(); ++i)
			{
				if (lock[i] + key[i] > 5)
				{
					good_key = false;
					break;
				}
			}
			if (good_key)
				++result;
		}

	return result;
}

int part_two()
{
	int result{ 0 };



	return result;
}