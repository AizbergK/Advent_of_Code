#include "../../timer_utility.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility input_timer, part1_timer, part2_timer;
// time to solve: 1.000ms

int main()
{
    std::string file_name = "./test.txt";
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input_timer.startTimer();
    get_data(file_name);
    input_timer.getDuration("data in + parsing");

    part1_timer.startTimer();
    part1();
    part1_timer.getDuration("part1");

    part2_timer.startTimer();
    part2();
    part2_timer.getDuration("part2");

    return 0;
}

void get_data(std::string &file_name)
{
    std::ifstream input;
    input.open("./test.txt");
}

void out_data()
{
    std::ofstream output;
    output.open("./output.txt");
}

void part1()
{
    return;
}

void part2()
{
    return;
}