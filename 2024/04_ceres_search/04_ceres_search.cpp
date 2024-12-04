#include "../../other/include_everything.h"
import std;

void read_data(std::vector<std::string>& data, int& line_count);
int part_one(std::vector<std::string> &data, int line_count);
int part_two(std::vector<std::string> &data, int line_count);
int check_word_x_mas(std::string& word1, std::string& word2);
int check_word_xmas(std::string& word);

int main()
{
	std::vector<std::string> data;
	int line_count;
	int part_one_result{ 0 }, part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;
	// 0.2170ms  : data in + parsing
	// 2.0651ms  : part1
	// 0.6736ms  : part2
	input_timer.startTimer();
	read_data(data, line_count);
	input_timer.getDuration();

	part1_timer.startTimer();
	part_one_result = part_one(data, line_count);
	part1_timer.getDuration();

	part2_timer.startTimer();
	part_two_result = part_two(data, line_count);
	part2_timer.getDuration();

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	return 0;
}

void read_data(std::vector<std::string> &data, int &line_count)
{
	std::ifstream input(__INPUT_PATH);
	std::string line;
	int i;
	for (i = 0; std::getline(input, line); ++i)
	{
		line = "PPP" + line + "PPP";
		data.emplace_back(line);

	}
	line_count = i;
	line = std::string(line.size(), 'P');
	for (int i{ 0 }; i != 3; ++i)
		data.emplace_back(line);
}

int part_one(std::vector<std::string>& data, int line_count)
{
	int result = 0;
	std::string word;
	for (int i{ 0 }; i != line_count; ++i)
	{
		for (int j{ 3 }; j != line_count + 3; ++j)
		{
			word = std::string(1, data[i][j]) + data[i][j + 1] + data[i][j + 2] + data[i][j + 3];
			result += check_word_xmas(word);
			word = std::string(1, data[i][j]) + data[i + 1][j] + data[i + 2][j] + data[i + 3][j];
			result += check_word_xmas(word);
			word = std::string(1, data[i][j]) + data[i + 1][j + 1] + data[i + 2][j + 2] + data[i + 3][j + 3];
			result += check_word_xmas(word);
			word = std::string(1, data[i][j]) + data[i + 1][j - 1] + data[i + 2][j - 2] + data[i + 3][j - 3];
			result += check_word_xmas(word);
		}
	}

	return result;
}

int part_two(std::vector<std::string>& data, int line_count)
{
	int result = 0;
	std::string word1, word2;
	for (int i{ 0 }; i != line_count; ++i)
	{
		for (int j{ 3 }; j != line_count + 3; ++j)
		{
			word1 = std::string(1, data[i][j]) + data[i + 1][j + 1] + data[i + 2][j + 2];
			word2 = std::string(1, data[i][j + 2]) + data[i + 1][j + 1] + data[i + 2][j];
			result += check_word_x_mas(word1, word2);
		}
	}

	return result;
}

int check_word_xmas(std::string &word)
{
	if (word == "XMAS" || word == "SAMX") return 1;

	return 0;
}

int check_word_x_mas(std::string& word1, std::string& word2)
{
	if ((word1 == "MAS" || word1 == "SAM") && (word2 == "MAS" || word2 == "SAM")) return 1;

	return 0;
}