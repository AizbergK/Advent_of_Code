#include "../../other/include_everything.h"
import std;

struct Pt
{
	int x;
	int y;
	int z;
};

typedef std::vector<Pt> data_type;

void run_program(std::string input, std::u8string comment, bool is_test_data);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data, bool is_test_data);
std::uint64_t part_two(data_type& data);

int main()
{
	// 176.90µs : input input day08
	// 17.293ms : part1
	// 43.956ms : part2
	run_program(__TEST_PATH, u8"test input", true);
	run_program(__INPUT_PATH, u8"input day08", false);
	return 0;
}

void run_program(std::string input, std::u8string comment, bool is_test_data)
{

	std::uint64_t part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	data_type data;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data, is_test_data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);
	if (input.bad())
	{
		std::cout << "cant open";
	}

	std::string temp;
	while (std::getline(input, temp))
	{
		int x, y, z;

		char const * ptr{ temp.data() };
		char const * const end{ ptr + temp.size() };
		auto res{ std::from_chars(ptr, end, x) };
		ptr = res.ptr + 1;
		res = std::from_chars(ptr, end, y);
		ptr = res.ptr + 1;
		res = std::from_chars(ptr, end, z);

		data.emplace_back(x, y, z);
	}
}

std::uint64_t part_one(data_type& data, bool is_test_data)
{
	std::uint64_t result{ 1 };

	using node = std::pair<std::int64_t, std::pair<int, int>>;
	std::priority_queue<node, std::vector<node>, std::greater<node>> distances;

	int loop_max{ is_test_data ? 10 : 1000 };
	std::vector<std::vector<int>> networks{};

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ i + 1 }; j < data.size(); ++j)
		{
			std::int64_t const x{ data[i].x - data[j].x };
			std::int64_t const y{ data[i].y - data[j].y };
			std::int64_t const z{ data[i].z - data[j].z };
			std::int64_t const distance{ x * x + y * y + z * z };
			distances.emplace(node(distance, std::pair<int, int>(i, j)));
		}
		networks.emplace_back(std::vector<int>{ i });
	}

	for(int a{ 0 }; a < loop_max; ++a)
	{
		auto [_, edge] = distances.top(); distances.pop();
		int first_idx{ -1 }, second_idx{ -1 };

		for (int i{ 0 }; i < networks.size(); ++i)
		{
			if (std::find(networks[i].begin(), networks[i].end(), edge.first) != networks[i].end())
			{
				first_idx = i;
			}
			if (std::find(networks[i].begin(), networks[i].end(), edge.second) != networks[i].end())
			{
				second_idx = i;
			}
		}

		if (first_idx != second_idx)
		{
			networks[first_idx].insert(networks[first_idx].end(), networks[second_idx].begin(), networks[second_idx].end());
			networks[second_idx].clear();
		}
	}


	int largest{ 0 }, second{ 0 }, third{ 0 };
	for (auto& network : networks)
	{
		if (network.size() > largest)
		{
			third = second;
			second = largest;
			largest = network.size();
		}
		else if (network.size() > second)
		{
			third = second;
			second = network.size();
		}
		else if (network.size() > third)
		{
			third = network.size();
		}
	}

	result = largest * second * third;

	return result;
}

std::uint64_t part_two(data_type& data)
{
	std::uint64_t result{ 1 };

	using node = std::pair<std::int64_t, std::pair<int, int>>;
	std::priority_queue<node, std::vector<node>, std::greater<node>> distances;
	std::vector<std::vector<int>> networks{};

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ i + 1 }; j < data.size(); ++j)
		{
			std::int64_t const x{ data[i].x - data[j].x };
			std::int64_t const y{ data[i].y - data[j].y };
			std::int64_t const z{ data[i].z - data[j].z };
			std::int64_t const distance{ x * x + y * y + z * z };
			distances.emplace(node(distance, std::pair<int, int>(i, j)));
		}
		networks.emplace_back(std::vector<int>{ i });
	}
	std::uint64_t last_x_one, last_x_two;

	std::size_t remaining_sets{ networks.size() };
	while (remaining_sets > 1)
	{
		auto [_, edge] = distances.top(); distances.pop();
		int first_idx{ -1 }, second_idx{ -1 };

		for (int i{ 0 }; i < networks.size(); ++i)
		{
			if (std::find(networks[i].begin(), networks[i].end(), edge.first) != networks[i].end())
			{
				first_idx = i;
			}
			if (std::find(networks[i].begin(), networks[i].end(), edge.second) != networks[i].end())
			{
				second_idx = i;
			}
		}

		if (first_idx != second_idx)
		{
			networks[first_idx].insert(networks[first_idx].end(), networks[second_idx].begin(), networks[second_idx].end());
			networks[second_idx].clear();
			last_x_one = data[edge.first].x;
			last_x_two = data[edge.second].x;
			--remaining_sets;
		}
	}

	result = last_x_one * last_x_two;

	return result;
}