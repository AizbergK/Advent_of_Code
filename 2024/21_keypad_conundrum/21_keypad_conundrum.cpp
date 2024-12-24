#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<std::string>& data);
int part_one(std::vector<std::string>& data);
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

constexpr std::array<std::array<const char*, 11>, 11> keypad_distances{
						// To:    A         0          1          2          3          4          5          6          7          8          9
	std::array<const char*, 11>{ "",        "<",       "^<<",     "^<",      "^",       "^^<<",    "^^<",    "^^",       "^^^<<",   "^^^<",    "^^^" },     // from A
	std::array<const char*, 11>{ ">",       "",        "^<",      "^",       "^>",      "^^<",     "^^",      "^^>",     "^^^<",    "^^^",     "^^^>" },    // from 0
	std::array<const char*, 11>{ ">>v",     ">v",      "",        ">",       ">>",      "^",       "^>",      "^>>",     "^^",      "^^>",     "^^>>" },    // from 1
	std::array<const char*, 11>{ "v>",      "v",       "<",       "",        ">",       "^<",      "^",       "^>",      "^^<",     "^^",      "^^>" },     // from 2
	std::array<const char*, 11>{ "v",       "v<",      "<<",      "<",       "",        "^<<",     "^<",      "^",       "^^<<",    "^^<",     "^^" },      // from 3
	std::array<const char*, 11>{ ">>vv",    ">vv",     "v",       "v>",      "v>>",     "",        ">",       ">>",      "^",       "^>",      "^>>" },     // from 4
	std::array<const char*, 11>{ "vv>",     "vv",      "v<",      "v",       "v>",      "<",       "",        ">",       "^<",      "^",       "^>" },      // from 5
	std::array<const char*, 11>{ "vv",      "vv<",     "v<<",     "v<",      "v",       "<<",      "<",       "",        "^<<",     "^<",      "^" },       // from 6
	std::array<const char*, 11>{ ">>vvv",   ">vvv",    "vv",      "vv>",     "vv>>",    "v",       "v>",      "v>>",     "",        ">",       ">>" },      // from 7
	std::array<const char*, 11>{ "vvv>",    "vvv",     "vv<",     "vv",      "vv>",     "v<",      "v",       "v>",      "<",       "",        ">" },       // from 8
	std::array<const char*, 11>{ "vvv",     "vvv<",    "vv<<",    "vv<",     "vv",      "v<<",     "v<",      "v",       "<<",      "<",       "" }         // from 9
};

constexpr std::array<std::array<const char*, 5>, 5> arrow_distances{
					  // To:    A       <       >       ^       v
	std::array<const char*, 5>{ "",		"v<<",	"v",	"<",	"v<" },		//A (starting point)
	std::array<const char*, 5>{ ">>^",	"",		">>",	">^",	">" },		//<
	std::array<const char*, 5>{ "^",	"<<",	"",		"<^",	"<" },		//>
	std::array<const char*, 5>{ ">",	"v<",	"v>",	"",		"v" },		//^
	std::array<const char*, 5>{ ">^",	"<",	">",	"^",	"" }		//v
};

void run_program(std::string input, std::u8string comment)
{
	std::vector<std::string> data;

	long long part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, data);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two();
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<std::string>& data)
{
	std::ifstream input(input_path);
	std::string temp;
	while (std::getline(input, temp))
	{
		data.emplace_back(temp);
	}
}

int arrowpad(char to_search)
{
	switch (to_search)
	{
	case 'A':
		return 0;
	case '<':
		return 1;
	case '>':
		return 2;
	case '^':
		return 3;
	case 'v':
		return 4;
	} 
}

std::string distance_from_to_keypad(char from_ch, char to_ch)
{
	std::string value{ "" };
	int to{ to_ch - '0' + 1 };
	int from{ from_ch - '0' + 1 };

	if (to > 10) to = 0;
	if (from > 10) from = 0;

	return keypad_distances[from][to];
}

std::string distance_from_to_arrow(char from_ch, char to_ch)
{
	std::string value{ "" };

	int to{ arrowpad(to_ch) };
	int from{ arrowpad(from_ch) };

	return arrow_distances[from][to];
}

int part_one(std::vector<std::string>& data)
{
	int result{ 0 };

	for (auto& row : data)
	{
		std::string final_string{ "" };
		char from{ 'A' };
		for (int i{ 0 }; i != row.size(); ++i)
		{
			std::string temp_keypad{ "" };
			if(i == 0)
				temp_keypad += distance_from_to_keypad(from , row[i]) + "A";
			else
				temp_keypad += distance_from_to_keypad(row[i - 1] , row[i]) + "A";
			for (int j{ 0 }; j != temp_keypad.size(); ++j)
			{
				std::string temp_arrows1{ "" };
				if (j == 0)
					temp_arrows1 += distance_from_to_arrow(from , temp_keypad[j]) + "A";
				else
					temp_arrows1 += distance_from_to_arrow(temp_keypad[j - 1] , temp_keypad[j]) + "A";
				for (int k{ 0 }; k != temp_arrows1.size(); ++k)
				{
					std::string temp_arrows2{ "" };
					if (k == 0)
						temp_arrows2 += distance_from_to_arrow(from , temp_arrows1[k]) + "A";
					else
						temp_arrows2 += distance_from_to_arrow(temp_arrows1[k - 1] , temp_arrows1[k]) + "A";
					final_string += temp_arrows2;
				}
			}
		}
		std::cout << final_string << " : " << final_string.size()<< " - " << std::stoi(row) << '\n';
		result += final_string.size() * std::stoi(row);
	}

	return result;
}

int part_two()
{
	int result{ 0 };



	return result;
}