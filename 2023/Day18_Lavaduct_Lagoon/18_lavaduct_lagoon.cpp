#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

TimerUtility part1_timer, part2_timer, data_timer;
// 0.2588ms  : data in + parsing
// 0.0312ms  : part1
// 0.0242ms  : part2

void get_data(std::vector<std::pair<char, int>> *, std::vector<std::pair<char, int>> *, std::string);
long long get_area(std::vector<std::pair<char, int>> *);

int main()
{
    std::string file_name = "./input.txt";
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<std::pair<char, int>> dir_l_arr, dir_l_arr_two;
    long long part1, part2;

    data_timer.startTimer();
    get_data(&dir_l_arr, &dir_l_arr_two, file_name);
    data_timer.getDuration("data in + parsing");

    part1_timer.startTimer();
    part1 = get_area(&dir_l_arr);
    part1_timer.getDuration("part1");

    part2_timer.startTimer();
    part2 = get_area(&dir_l_arr_two);
    part2_timer.getDuration("part2");

    std::cout << part1 << '\n' << part2 << '\n';

    return 0;
}

void get_data(std::vector<std::pair<char, int>> *dir_l_arr, std::vector<std::pair<char, int>> *dir_l_arr_two,
              std::string file_name)
{
    std::ifstream input;
    input.open(file_name);
    std::pair<char, int> dir_l;
    std::string hex;
    int dig_length, dig_dir;

    while (!input.eof())
    {
        input >> dir_l.first;
        input >> dir_l.second;
        dir_l_arr->push_back(dir_l);
        input >> hex;
        dir_l.second = std::stoi(hex.substr(2, 5), nullptr, 16);
        dig_dir = hex[7] - '0';
        switch (dig_dir)
        {
        case 0:
            dir_l.first = 'R';
            break;
        case 1:
            dir_l.first = 'D';
            break;
        case 2:
            dir_l.first = 'L';
            break;
        case 3:
            dir_l.first = 'U';
            break;
        }
        dir_l_arr_two->push_back(dir_l);
    }

    return;
}

long long get_area(std::vector<std::pair<char, int>> *dir_l_arr)
{
    std::vector<std::pair<long long, long long>> all_points;
    long long sum_left = 0, sum_right = 0, start_x = 0, start_y = 0, perimeter = 0;

    std::pair<long long, long long> point;
    for (int i = 0; i < dir_l_arr->size(); i++)
    {
        switch ((*dir_l_arr)[i].first)
        {
        case 'L':
            start_x -= (*dir_l_arr)[i].second;
            break;
        case 'R':
            start_x += (*dir_l_arr)[i].second;
            break;
        case 'U':
            start_y -= (*dir_l_arr)[i].second;
            break;
        case 'D':
            start_y += (*dir_l_arr)[i].second;
            break;
        }
        point.first = start_y;
        point.second = start_x;
        perimeter += (*dir_l_arr)[i].second;
        all_points.push_back(point);
    }

    for (int i = 0; i < all_points.size() - 1; i++)
    {
        sum_left += all_points[i].first * all_points[i + 1].second;
        sum_right += all_points[i].second * all_points[i + 1].first;
    }

    sum_left += all_points[all_points.size() - 1].first * all_points[0].second;
    sum_right += all_points[all_points.size() - 1].second * all_points[0].first;

    return (std::abs(sum_left - sum_right) / 2 + (perimeter + 2) / 2);
}
