#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

TimerUtility program_timer;
// time to solve: 14.3289ms

std::pair<int, int> get_prediction(std::vector<int>);
std::vector<int> get_next_sequence(std::vector<int>);
bool is_sequence_zero(std::vector<int>);

int main()
{

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    int number, total_right = 0, total_left = 0;
    std::string line;
    std::vector<int> sequence;

    while (std::getline(input, line))
    {
        std::stringstream ss(line);
        while (ss >> number)
        {
            sequence.push_back(number);
        }
        total_right += get_prediction(sequence).second;
        total_left += get_prediction(sequence).first;
        sequence.clear();
    }

    std::cout << total_right << std::endl << total_left << std::endl;

    program_timer.getDuration();

    return 0;
}

std::pair<int, int> get_prediction(std::vector<int> sequence)
{

    std::vector<std::vector<int>> sequences_to_zero;
    std::pair<int, int> prediction = {0, 0};
    sequences_to_zero.push_back(sequence);

    while (!is_sequence_zero(sequences_to_zero.back()))
    {
        sequences_to_zero.push_back(get_next_sequence(sequences_to_zero.back()));
    }

    for (int i = sequences_to_zero.size() - 1; i >= 0; i--)
    {
        prediction.second += sequences_to_zero[i].back();
    }

    for (int i = sequences_to_zero.size() - 2; i >= 0; i--)
    {
        prediction.first = sequences_to_zero[i][0] - prediction.first;
    }

    return prediction;
}

std::vector<int> get_next_sequence(std::vector<int> sequence)
{

    std::vector<int> next_sequence;

    for (int i = 0; i < sequence.size() - 1; i++)
    {
        next_sequence.push_back(sequence[i + 1] - sequence[i]);
    }

    return next_sequence;
}

bool is_sequence_zero(std::vector<int> sequence)
{
    for (int i = 0; i < sequence.size(); i++)
    {
        if (sequence[i] != 0)
        {
            return false;
        }
    }
    return true;
}