#include "../../other/include_everything.h"
import std;

typedef std::unordered_set<std::string_view> towels_view_type;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::string& towels, towels_view_type& towel_views, std::vector<std::string>& to_check, std::vector<std::string_view>& to_check_views, int& max_towel_size);
std::pair<long long, long long> part_one_and_two(towels_view_type& towel_views, std::vector<std::string_view>& to_check_views, int max_towel_size);

int main()
{
	// 257.90µs  : data in + parsing
	// 1.6609ms  : part1 and part2
	run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day19");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	int max_towel_size{ 0 };
	std::string towels;
	towels_view_type towel_views;
	std::vector<std::string> to_check;
	std::vector<std::string_view> to_check_views;
	TimerUtility input_timer, part12_timer;

	input_timer.startTimer();
	read_data(input, towels, towel_views, to_check, to_check_views, max_towel_size);
	input_timer.getDuration(u8"input " + comment);

	part12_timer.startTimer();
	auto [part_one_result, part_two_result] = part_one_and_two(towel_views, to_check_views, max_towel_size);
	part12_timer.getDuration(u8"part1 and part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::string& towels, towels_view_type& towel_views, std::vector<std::string>& to_check, std::vector<std::string_view>& to_check_views, int& max_towel_size)
{
	std::ifstream input(input_path);
	std::getline(input, towels);
	std::string temp;
	towels += ", ";
	std::string_view towels_view{ towels };
	while (!towels_view.empty())
	{
		max_towel_size = max_towel_size > towels_view.find_first_of(',') ? max_towel_size : towels_view.find_first_of(',');
		towel_views.insert(towels_view.substr(0, towels_view.find_first_of(',')));
		towels_view = towels_view.substr(towels_view.find_first_of(' ') + 1);
	}
	std::getline(input, temp);
	while (std::getline(input, temp))
	{
		to_check.emplace_back(temp);
	}
	for(auto& str : to_check)
		to_check_views.emplace_back(std::string_view{ str });
}

long long check_towel(towels_view_type& towel_views, std::string_view towel, int max_towel_size)
{
	std::vector<long long> can_form{ std::vector<long long>(towel.size() + 1, 0) };
	can_form[0] = 1;
	for (int index{ 0 }; index != can_form.size(); ++index)
	{
		if (can_form[index] == 0) continue;

		for (int i{ 1 }; i <= max_towel_size; ++i)
		{
			if (index + i >= can_form.size()) break;
			can_form[index + i] += can_form[index] * (towel_views.find(towel.substr(index, i)) != towel_views.end());
		}
	}
	return can_form[can_form.size() - 1];
}

std::pair<long long, long long> part_one_and_two(towels_view_type& towel_views, std::vector<std::string_view>& to_check_views, int max_towel_size)
{
	long long result{ 0 }, total{ 0 };
	for (auto& towel : to_check_views)
	{
		auto got_result{ check_towel(towel_views, towel, max_towel_size) };
		result += got_result;
		total += (bool)got_result;
	}

	return std::pair<long long, long long>{ total, result };
}