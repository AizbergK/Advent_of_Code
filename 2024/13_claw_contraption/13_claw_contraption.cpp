#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::array<long long, 6>>& data);
long long part_one(std::vector<std::array<long long, 6>>& data);
long long part_two(std::vector<std::array<long long, 6>>& data);

int main()
{
	std::vector<std::array<long long, 6>> data;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 397.70µs  : data in + parsing
	// 2.3000µs  : part1
	// 2.5000µs  : part2
	input_timer.startTimer();
	read_data(data);
	input_timer.getDuration(u8"input day13");

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

void read_data(std::vector<std::array<long long, 6>>& data)
{
	std::ifstream input(__INPUT_PATH);
	std::string line;
	while (std::getline(input, line))
	{
		std::array<long long, 6> temp;
		temp[0] = std::stoi(line.substr(line.find_first_of('+'), line.find_first_of(',') - line.find_first_of('+')));
		line = line.substr(line.find_first_of(','));
		temp[1] = std::stoi(line.substr(line.find_first_of('+')));
		std::getline(input, line);
		temp[2] = std::stoi(line.substr(line.find_first_of('+'), line.find_first_of(',') - line.find_first_of('+')));
		line = line.substr(line.find_first_of(','));
		temp[3] = std::stoi(line.substr(line.find_first_of('+')));
		std::getline(input, line);
		temp[4] = std::stoi(line.substr(line.find_first_of('=') + 1, line.find_first_of(',') - line.find_first_of('=')));
		line = line.substr(line.find_first_of(','));
		temp[5] = std::stoi(line.substr(line.find_first_of('=') + 1));

		data.emplace_back(temp);
		std::getline(input, line);
	}

}

long long check_minimum_presses(long long pos_0, long long pos_1, long long pos_2, long long pos_3, long long pos_4, long long pos_5)
{
	long long x = 0, y = 0, temp1, temp2;
	temp1 = pos_3 * pos_4 - pos_2 * pos_5;
	temp2 = pos_3 * pos_0 - pos_2 * pos_1;
	if (temp1 % temp2 == 0) x = temp1 / temp2;
	temp1 = pos_1 * pos_4 - pos_0 * pos_5;
	temp2 = -temp2;
	if (temp1 % temp2 == 0) y = temp1 / temp2;

	if (x > 0 && y > 0 && x <= 100 && y <= 100) return x * 3 + y;

	return 0;
}

long long part_one(std::vector<std::array<long long, 6>>& data)
{
	long long result{ 0 };

	for (auto row : data)
		result += check_minimum_presses(row[0], row[1], row[2], row[3], row[4], row[5]);

	return result;
}

long long check_minimum_presses_part2(long long pos_0, long long pos_1, long long pos_2, long long pos_3, long long pos_4, long long pos_5)
{
	long long x = 0, y = 0, temp1, temp2;
	temp1 = pos_3 * pos_4 - pos_2 * pos_5;
	temp2 = pos_3 * pos_0 - pos_2 * pos_1;
	if (temp1 % temp2 == 0) x = temp1 / temp2;
	temp1 = pos_1 * pos_4 - pos_0 * pos_5;
	temp2 = -temp2;
	if (temp1 % temp2 == 0) y = temp1 / temp2;

	if (x > 0 && y > 0) return x * 3 + y;

	return 0;
}

long long part_two(std::vector<std::array<long long, 6>>& data)
{
	long long result{ 0 };

	for (auto row : data)
		result += check_minimum_presses_part2(row[0], row[1], row[2], row[3], row[4] + 10'000'000'000'000, row[5] + 10'000'000'000'000);

	return result;
}