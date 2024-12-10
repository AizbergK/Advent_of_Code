#include "../../other/include_everything.h"
import std;

typedef std::vector<std::pair<int, int>> data_struct;
			//idx-value, first-length, second-start position

void read_data(data_struct&, data_struct&);
long long part_one(data_struct, data_struct);
long long part_two(data_struct, data_struct);

int main()
{
	data_struct data_chunks, free_chunks;
	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 340.90µs  : data in + parsing
	// 186.20µs  : part1
	// 10.535ms  : part2
	input_timer.startTimer();
	read_data(data_chunks, free_chunks);
	input_timer.getDuration(u8"input day0");

	part1_timer.startTimer();
	part_one_result = part_one(data_chunks, free_chunks);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data_chunks, free_chunks);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(data_struct& data_chunks, data_struct& free_chunks)
{
	std::ifstream input(__INPUT_PATH);
	int pos = 0;
	std::string data;
	std::getline(input, data);
	for (int i{ 0 }; i != data.size(); ++i)
	{
		if (i % 2 == 0)
		{
			data_chunks.emplace_back(std::pair<int, int>{ data[i] - '0', pos });
		}
		else
		{
			free_chunks.emplace_back(std::pair<int, int>{ data[i] - '0', pos });
		}
		pos += data[i] - '0';
	}
}

long long part_one(data_struct data_chunks, data_struct free_chunks)
{
	long long result = 0, reverse_order = data_chunks.size() - 1;

	for (int i{ 0 }; i != free_chunks.size() && reverse_order > i; ++i)
	{
		while(free_chunks[i].first > 0)
		{
			if (data_chunks[reverse_order].first == 0)
				--reverse_order;
			result += free_chunks[i].second * reverse_order;
			--data_chunks[reverse_order].first;
			--free_chunks[i].first;
			++free_chunks[i].second;
		}
	}
	for (int i{ 0 }; i != data_chunks.size(); ++i)
	{
		while (data_chunks[i].first > 0)
		{
			result += data_chunks[i].second * i;
			--data_chunks[i].first;
			--free_chunks[i].second;
			++data_chunks[i].second;
		}
	}

	return result;
}

long long part_two(data_struct data_chunks, data_struct free_chunks)
{
	long long result = 0;

	for (auto i{ data_chunks.size() - 1 }; i != -1; --i)
	{
		for (auto j{ 0 }; j != i; ++j)
		{
			if (free_chunks[j].first >= data_chunks[i].first)
			{
				data_chunks[i].second = free_chunks[j].second;
				free_chunks[j].second += data_chunks[i].first;
				free_chunks[j].first -= data_chunks[i].first;
				break;
			}
		}
	}

	for (int i{ 0 }; i != data_chunks.size(); ++i)
	{
		while (data_chunks[i].first > 0)
		{
			result += data_chunks[i].second * i;
			--data_chunks[i].first;
			++data_chunks[i].second;
		}
	}

	return result;
}