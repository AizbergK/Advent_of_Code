#include "../../other/include_everything.h"
import std;

typedef std::vector<std::pair<int, int>> data_type;

void run_program(std::string input, std::u8string comment, bool test_data);
void read_data(std::string& input_path, data_type& data);
std::uint64_t part_one(data_type& data);
std::uint64_t part_two(data_type& data, bool test_data);

int main()
{
	// 711.90µs : input input day09
	// 63.300µs : part1
	// 42.078s  : part2 - brute force 10gb RAM
	run_program(__TEST_PATH, u8"test input", true);
	run_program(__INPUT_PATH, u8"input day09", false);
	return 0;
}

void run_program(std::string input, std::u8string comment, bool test_data)
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

	part2_timer.startTimer();
	part_two_result = part_two(data, test_data);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, data_type& data)
{
	std::ifstream input(input_path);

	std::string temp;
	while (std::getline(input, temp))
	{
		int x, y;

		char const* ptr{ temp.data() };
		char const* const end{ ptr + temp.size() };
		auto res{ std::from_chars(ptr, end, x) };
		ptr = res.ptr + 1;
		res = std::from_chars(ptr, end, y);

		data.emplace_back(x, y);
	}
}

std::uint64_t part_one(data_type& data)
{
	std::uint64_t result{ 0 };

	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ i + 1 }; j < data.size(); ++j)
		{
			std::uint64_t x_len{ std::abs(data[i].first - data[j].first) + 1ULL };
			std::uint64_t y_len{ std::abs(data[i].second - data[j].second) + 1ULL };
			std::uint64_t area{ x_len * y_len };
			if (area > result)
			{
				result = area;
			}
		}
	}

	return result;
}

std::uint64_t part_two(data_type& data, bool test_data)
{
	std::uint64_t result{ 0 };

	int mat_sz{ test_data ? 15 : 100'000 };
	std::vector<std::vector<int8_t>> matrix(mat_sz, std::vector<int8_t>(mat_sz, 0));
	
	int range_start{ 0 }, range_end{ 0 }, const_x{ -1 }, const_y{ -1 };
	auto& st_node = data.back();
	auto& nd_node = data.front();
	if (nd_node.first - st_node.first == 0)
	{
		const_x = st_node.first;
		const_y = -1;
		if (nd_node.second - st_node.second < 0)
		{
			range_start = nd_node.second;
			range_end = st_node.second;
		}
		else
		{
			range_start = st_node.second;
			range_end = nd_node.second;
		}
	}
	else
	{
		const_x = -1;
		const_y = nd_node.second;
		if (nd_node.first - st_node.first < 0)
		{
			range_start = nd_node.first;
			range_end = st_node.first;
		}
		else
		{
			range_start = st_node.first;
			range_end = nd_node.first;
		}
	}
	if (const_x == -1)
	{
		for (int i{ range_start }; i <= range_end; ++i)
		{
			matrix[i][const_y] = 1;
		}
	}
	else
	{
		for (int i{ range_start }; i <= range_end; ++i)
		{
			matrix[const_x][i] = 1;
		}
	}
	for (int i{ 1 }; i < data.size(); ++i)
	{
		int range_start{ 0 }, range_end{ 0 }, const_x{ -1 }, const_y{ -1 };
		auto& st_node = data[i - 1];
		auto& nd_node = data[i];
		if (nd_node.first - st_node.first == 0)
		{
			const_x = st_node.first;
			const_y = -1;
			if (nd_node.second - st_node.second < 0)
			{
				range_start = nd_node.second;
				range_end = st_node.second;
			}
			else
			{
				range_start = st_node.second;
				range_end = nd_node.second;
			}
		}
		else
		{
			const_x = -1;
			const_y = nd_node.second;
			if (nd_node.first - st_node.first < 0)
			{
				range_start = nd_node.first;
				range_end = st_node.first;
			}
			else
			{
				range_start = st_node.first;
				range_end = nd_node.first;
			}
		}
		if (const_x == -1)
		{
			for (int i{ range_start }; i <= range_end; ++i)
			{
				matrix[i][const_y] = 1;
			}
		}
		else
		{
			for (int i{ range_start }; i <= range_end; ++i)
			{
				matrix[const_x][i] = 1;
			}
		}
	}

	std::queue<std::pair<int, int>> flood_fill_queue;
	flood_fill_queue.push({ 0, 0 });
	matrix[0][0] = 2;

	while (!flood_fill_queue.empty()) 
	{
		auto [r, c] = flood_fill_queue.front();
		flood_fill_queue.pop();

		if (r > 0 && matrix[r - 1][c] == 0) 
		{ 
			matrix[r - 1][c] = 2; 
			flood_fill_queue.push({ r - 1, c }); 
		}
		if (r < mat_sz - 1 && matrix[r + 1][c] == 0)
		{ 
			matrix[r + 1][c] = 2; 
			flood_fill_queue.push({ r + 1, c }); 
		}
		if (c > 0 && matrix[r][c - 1] == 0)
		{ 
			matrix[r][c - 1] = 2; 
			flood_fill_queue.push({ r, c - 1 }); 
		}
		if (c < mat_sz - 1 && matrix[r][c + 1] == 0)
		{ 
			matrix[r][c + 1] = 2; 
			flood_fill_queue.push({ r, c + 1 }); 
		}
	}
	
	for (int i{ 0 }; i < data.size(); ++i)
	{
		for (int j{ i + 1 }; j < data.size(); ++j)
		{
			int x1{ data[i].first }, x2{ data[j].first }, y1{ data[i].second }, y2{ data[j].second };
			if (x1 > x2) std::swap(x1, x2);
			if (y1 > y2) std::swap(y1, y2);

			bool is_outside{ false };
			for (int x{ x1 }; x <= x2 && !is_outside; ++x)
			{
				if (matrix[x][y1] == 2)
				{
					is_outside = true;
					break;
				}
				if (matrix[x][y2] == 2)
				{
					is_outside = true;
					break;
				}
			}

			for (int y{ y1 }; y <= y2 && !is_outside; ++y)
			{
				if (matrix[x1][y] == 2)
				{
					is_outside = true;
					break;
				}
				if (matrix[x2][y] == 2)
				{
					is_outside = true;
					break;
				}
			}

			if (!is_outside)
			{
				std::uint64_t x_len{ std::abs(data[i].first - data[j].first) + 1ULL };
				std::uint64_t y_len{ std::abs(data[i].second - data[j].second) + 1ULL };
				std::uint64_t area{ x_len * y_len };
				if (area > result)
				{
					result = area;
				}
			}
		}
	}


	return result;
}