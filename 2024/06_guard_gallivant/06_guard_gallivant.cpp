#include "../../other/include_everything.h"
import std;

enum class direction {up, down, left, right};

class Guard {

private:
	std::pair<int, int> position;
	direction dir = direction::up;

	void turn_right()
	{
		switch (dir)
		{
		case direction::up:
			this->dir = direction::right;
			break;
		case direction::right:
			this->dir = direction::down;
			break;
		case direction::down:
			this->dir = direction::left;
			break;
		case direction::left:
			this->dir = direction::up;
			break;
		}
	}



public:
	bool in_bounds(int bound_x, int bound_y)
	{
		if (this->position.first < bound_x && this->position.first > -1 && this->position.second < bound_y && this->position.second > -1)
			return true;

		return false;
	}

	bool move_forward(std::vector<std::string>& data)
	{
		switch (this->dir)
		{
		case direction::up:
			--this->position.first;
			break;
		case direction::right:
			++this->position.second;
			break;
		case direction::down:
			++this->position.first;
			break;
		case direction::left:
			--this->position.second;
			break;
		}
		if (!this->in_bounds(data[0].size(), data.size())) 
		{
			return false;
		}

		if (data[this->position.first][this->position.second] == '#')
		{
			switch (this->dir)
			{
			case direction::up:
				++this->position.first;
				break;
			case direction::right:
				--this->position.second;
				break;
			case direction::down:
				--this->position.first;
				break;
			case direction::left:
				++this->position.second;
				break;
			}
			this->turn_right();
		}

		return true;
	}

	void set_pos(int x, int y)
	{
		this->position.first = x;
		this->position.second = y;
	}

	int get_x() { return this->position.first; }
	int get_y() { return this->position.second; }
};

void read_data(std::vector<std::string>& data, Guard& the_guard);
int part_one(std::vector<std::string>& data, Guard the_guard);
int part_two(std::vector<std::string>& data, Guard the_guard);

int main()
{
	std::vector<std::string> data;
	Guard the_guard;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 213.60µs  : data in + parsing
	// 45.400µs  : part1
	// 37.671ms  : part2
	input_timer.startTimer();
	read_data(data, the_guard);
	input_timer.getDuration();

	part1_timer.startTimer();
	part_one_result = part_one(data, the_guard);
	part1_timer.getDuration();

	part2_timer.startTimer();
	part_two_result = part_two(data, the_guard);
	part2_timer.getDuration();

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::vector<std::string>& data, Guard& the_guard)
{
	std::ifstream input(__INPUT_PATH);
	std::string line;
	while (std::getline(input, line))
	{
		data.emplace_back(line);
	}
	for (int i{ 0 }; i != data.size(); ++i)
	{
		for (int j{ 0 }; j != data[0].size(); ++j)
		{
			if (data[i][j] == '^') 
			{
				the_guard.set_pos(i, j);
				return;
			}
		}
	}
}

int part_one(std::vector<std::string>& data, Guard the_guard)
{
	int result = 1, total_hash = 0;

	while (the_guard.move_forward(data))
	{
		if (data[the_guard.get_x()][the_guard.get_y()] != '#')
		{
			data[the_guard.get_x()][the_guard.get_y()] = 'K';
		}
	}
	for (auto row : data)
		for (auto ch : row)
			result += ch == 'K';

	return result;
}

int part_two(std::vector<std::string>& data, Guard the_guard)
{
	int result = 0, step = 0;

	for (int i{ 0 }; i != data.size(); ++i)
	{
		for (int j{ 0 }; j != data[i].size(); ++j)
		{
			if (data[i][j] == '#' || data[i][j] == '^' || data[i][j] == '.') continue;
			char prev_char = data[i][j];
			data[i][j] = '#';
			Guard run_maze = the_guard;
			step = 0;
			while (run_maze.move_forward(data) && step < 10'000)
			{
				++step;
			}
			if (step == 10'000) ++result;
			data[i][j] = prev_char;
		}
	}


	return result;
}