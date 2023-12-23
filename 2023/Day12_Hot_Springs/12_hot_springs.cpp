#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

#include "../../timer_utility.h"

TimerUtility program_timer, run_timer;
// time to solve: 4.8986ms

void parse_input_line(std::ifstream *, std::string *, std::vector<std::pair<int, int>> *);
void get_numbers_max_pos(std::string *, std::vector<std::pair<int, int>> *);
void unfold_line(std::string *, std::vector<std::pair<int, int>> *);
int recursive_count(int, long long *, std::string *, std::vector<std::pair<int, int>> *, int);

int main()
{
    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    std::ofstream output;
    // output.open("./output.txt");

    long long accumulator_folded = 0, accumulator_unfolded = 0, temp_test, count = 1;
    std::vector<std::pair<int, int>> max_index_pos;
    std::string test_string;

    while (!input.eof())
    {
        // part1
        //  std::cout << count++ << std::endl;
        parse_input_line(&input, &test_string, &max_index_pos);
        temp_test = accumulator_folded;
        recursive_count(0, &accumulator_folded, &test_string, &max_index_pos, 0);
        // output << accumulator_folded - temp_test << std::endl;

        // part2
        unfold_line(&test_string, &max_index_pos);
        run_timer.startTimer();
        recursive_count(0, &accumulator_unfolded, &test_string, &max_index_pos, 0);
        run_timer.getDuration();
    }

    std::cout << accumulator_folded << std::endl;
    std::cout << accumulator_unfolded << std::endl;

    program_timer.getDuration();

    return 0;
}

void parse_input_line(std::ifstream *input, std::string *condition_row, std::vector<std::pair<int, int>> *numbers)
{

    std::string numbers_string, condition_str;
    int number = 0, condition_row_size;
    numbers->clear();
    (*input) >> (*condition_row);
    (*input) >> numbers_string;
    condition_row_size = (*condition_row).length();
    std::pair<int, int> number_max_loc;

    for (int i = 0; i < numbers_string.length(); i++)
    {
        if (numbers_string[i] != ',')
        {
            number *= 10;
            number += numbers_string[i] - 48;
        }
        else
        {
            number_max_loc.first = number;
            number_max_loc.second = 0;
            numbers->push_back(number_max_loc);
            number = 0;
        }
    }
    number_max_loc.first = number;
    number_max_loc.second = 0;
    numbers->push_back(number_max_loc);

    get_numbers_max_pos(condition_row, numbers);

    return;
}

void get_numbers_max_pos(std::string *condition_row, std::vector<std::pair<int, int>> *numbers)
{

    int max_pos;

    for (int i = 0; i < numbers->size(); i++)
    {
        max_pos = 0;
        for (int j = i; j < numbers->size(); j++)
        {
            max_pos += (*numbers)[j].first;
        }
        max_pos += (((*numbers).size()) - i - 1);
        (*numbers)[i].second = (*condition_row).length() - max_pos;
    }

    return;
}

void unfold_line(std::string *condition_row, std::vector<std::pair<int, int>> *numbers)
{

    std::string original_row = (*condition_row);
    int numbers_length = numbers->size();

    for (int i = 0; i < 4; i++)
    {
        (*condition_row) += "?";
        (*condition_row) += original_row;
        for (int j = 0; j < numbers_length; j++)
        {
            numbers->push_back((*numbers)[j]);
        }
    }

    get_numbers_max_pos(condition_row, numbers);

    return;
}

int recursive_count(int test_iterator, long long *accumulator, std::string *test_string,
                    std::vector<std::pair<int, int>> *numbers, int numbers_index)
{

    if (test_iterator > (*numbers)[numbers_index].second)
    {
        return 0;
    }
    int substr_length = (*numbers)[numbers_index].first;
    bool is_same = true, is_not_last_in_numbers = numbers_index != (*numbers).size() - 1;
    for (int i = 0; i < substr_length; i++)
    {
        if ((*test_string)[i + test_iterator] != '?' && (*test_string)[i + test_iterator] == '.')
        {
            is_same = false;
            break;
        }
    }
    if (is_not_last_in_numbers && ((*test_string)[substr_length + test_iterator] != '.') &&
        ((*test_string)[substr_length + test_iterator] != '?'))
    {
        is_same = false;
    }
    if (test_iterator > 0 && (*test_string)[test_iterator - 1] == '#')
    {
        return 0;
    }
    if (!is_not_last_in_numbers)
    {
        for (int i = substr_length + test_iterator; i < (*test_string).length(); i++)
        {
            if ((*test_string)[i] == '#')
            {
                is_same = false;
                break;
            }
        }
    }
    if (is_same && is_not_last_in_numbers)
    {
        (*accumulator) +=
            recursive_count(test_iterator + substr_length + 1, accumulator, test_string, numbers, numbers_index + 1);
    }
    if (test_iterator < (*numbers)[numbers_index].second)
    {
        (*accumulator) += recursive_count(test_iterator + 1, accumulator, test_string, numbers, numbers_index);
    }
    if (is_same && !is_not_last_in_numbers)
    {
        return 1;
    }

    return 0;
}