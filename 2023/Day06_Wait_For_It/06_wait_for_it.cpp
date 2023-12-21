#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility program_timer;
// time to solve: 1.7252ms

long long int race_win_nr(long long int, long long int);
long long concat_strings_to_int(std::vector<std::string> *);

int main()
{

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    std::string temp_string;
    int temp_number;

    std::vector<std::string> time, dist;

    input >> temp_string;
    input >> temp_string;
    while (temp_string != "Distance:")
    {
        time.push_back(temp_string);
        input >> temp_string;
    }
    while (input >> temp_string)
    {
        dist.push_back(temp_string);
    }

    int total = 1;

    for (int i = 0; i < time.size(); i++)
    {
        total *= race_win_nr(std::stoll(time[i]), std::stoll(dist[i]));
    }

    std::cout << total << std::endl
              << race_win_nr(concat_strings_to_int(&time), concat_strings_to_int(&dist)) << std::endl;

    program_timer.getDuration();

    return 0;
}

long long concat_strings_to_int(std::vector<std::string> *number_string)
{

    long long concat_number;
    std::string concat_string = "";

    for (int i = 0; i < number_string->size(); i++)
    {
        concat_string += (*number_string)[i];
    }

    concat_number = std::stoll(concat_string);

    return concat_number;
}

long long int race_win_nr(long long int time, long long int distance)
{

    long long int total, start_interval;

    for (int i = (distance / time); i < time; i++)
    {
        if ((time - i) * i > distance)
        {
            total = time + 1 - 2 * i;
            break;
        }
    }

    return total;
}