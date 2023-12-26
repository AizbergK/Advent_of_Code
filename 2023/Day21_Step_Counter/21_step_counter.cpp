#include "../../timer_utility.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility input_timer, part1_timer, part2_timer;
// 0.3125ms  : data in + parsing
// 1.4440ms  : part1
// 18.245ms  : part2

// After visualizing the output for part 1 and analyzing the input a bit I figured out that the resulting
// pattern for part 2 would be a very big rhombus.

// Part 2 algorithm(all hardcoded numbers resulted from math in my notebook):
//     -calculate the number of tiles per fully traversed tile and multiply with total number of fully travesed tiles;
//     -calculate the number of tiles in the topmost, leftmost, dwnmost and rightmost tiles using the algorithm
//         devised for part 1 and a known number of steps left;
//     -calculate the number of tiles on the diagonals using the alg from part 1 and a known number of steps;

struct coords
{
    int x;
    int y;
};

void get_data(std::string &, std::vector<std::vector<char>> *, coords *);
void out_data(std::vector<std::vector<char>> *);
void part1(long long *, std::vector<std::vector<char>>, coords, int);
void find_all_positions(int *, std::vector<std::pair<coords, int>> *, std::vector<std::vector<char>> *, int);
void check_offsets(std::vector<std::vector<char>> *, coords, std::vector<std::pair<coords, int>> *, int);
void check_offset(int, int, std::vector<std::vector<char>> *, coords, std::vector<std::pair<coords, int>> *, int);
void part2(long long *, std::vector<std::vector<char>>, coords, int);
void find_all_positions_pt2(std::vector<std::pair<coords, int>> *, std::vector<std::vector<char>> *, int);
void check_offsets_pt2(std::vector<std::vector<char>> *, coords, std::vector<std::pair<coords, int>> *, int);
void check_offset_pt2(int, int, std::vector<std::vector<char>> *, coords, std::vector<std::pair<coords, int>> *, int);
int cout_not_rocks(std::vector<std::vector<char>> *);
long long get_diagonals_to_subtract(std::vector<std::vector<char>> *);
int count_by_pos_and_steps(int, int, int, std::vector<std::vector<char>>);

int main()
{
    std::string file_name = "./input.txt";
    int steps = file_name == "./test.txt" ? 6 : 64;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::vector<std::vector<char>> walk_map;
    long long part1_total = 0, part2_total = 0;
    coords start;

    input_timer.startTimer();
    get_data(file_name, &walk_map, &start);
    input_timer.getDuration("data in + parsing");

    part1_timer.startTimer();
    part1(&part1_total, walk_map, start, steps);
    part1_timer.getDuration("part1");

    part2_timer.startTimer();
    part2(&part2_total, walk_map, start, steps * 5);
    part2_timer.getDuration("part2");

    std::cout << part1_total << '\n' << part2_total << '\n';

    return 0;
}

void get_data(std::string &file_name, std::vector<std::vector<char>> *walk_map, coords *start)
{
    std::ifstream input;
    input.open(file_name);
    std::string temp_string;

    getline(input, temp_string);
    input.close();

    walk_map->resize(temp_string.length());
    for (auto &&str : *walk_map)
    {
        str.resize(temp_string.length());
    }

    input.open(file_name);

    for (int j = 0; j < walk_map->size(); j++)
    {
        getline(input, temp_string);
        for (int i = 0; i < (*walk_map)[j].size(); i++)
        {
            if (temp_string[i] == 'S')
            {
                start->x = j;
                start->y = i;
                (*walk_map)[j][i] = '.';
            }
            else
            {
                (*walk_map)[j][i] = temp_string[i];
            }
        }
    }
}

void out_data(std::vector<std::vector<char>> *walk_map)
{
    std::ofstream output;
    output.open("./output.txt");

    for (auto &&line : *walk_map)
    {
        for (auto &&chr : line)
        {
            output << chr;
        }
        output << '\n';
    }
    output.close();
}

void part1(long long *total, std::vector<std::vector<char>> walk_map, coords start, int steps)
{
    int count = 0;
    coords temp_start_point;
    temp_start_point.x = 0;
    temp_start_point.y = 0;
    std::vector<std::pair<coords, int>> all_positions;
    all_positions.emplace_back(start, steps);
    find_all_positions(&count, &all_positions, &walk_map, 0);

    *total = count;
}

void find_all_positions(int *count, std::vector<std::pair<coords, int>> *all_positions,
                        std::vector<std::vector<char>> *walk_map, int curr_index)
{
    coords curr_pos = (*all_positions)[curr_index].first, temp_pos;
    if (curr_index == all_positions->size() || !(*all_positions)[curr_index].second)
    {
        all_positions->erase(all_positions->begin());
        for (auto &&pos : *all_positions)
        {
            if (pos.second % 2 == 0)
            {
                (*count)++;
            }
            else
            {
                (*walk_map)[pos.first.x][pos.first.y] = '.';
            }
        }
        return;
    }
    check_offsets(walk_map, curr_pos, all_positions, curr_index);

    return find_all_positions(count, all_positions, walk_map, curr_index + 1);
}

void check_offsets(std::vector<std::vector<char>> *walk_map, coords curr_pos,
                   std::vector<std::pair<coords, int>> *all_positions, int curr_index)
{
    check_offset(1, 0, walk_map, curr_pos, all_positions, curr_index);
    check_offset(0, 1, walk_map, curr_pos, all_positions, curr_index);
    check_offset(-1, 0, walk_map, curr_pos, all_positions, curr_index);
    check_offset(0, -1, walk_map, curr_pos, all_positions, curr_index);
}

void check_offset(int x_ofs, int y_ofs, std::vector<std::vector<char>> *walk_map, coords curr_pos,
                  std::vector<std::pair<coords, int>> *all_positions, int curr_index)
{
    coords temp_pos;
    if (curr_pos.x + x_ofs < walk_map->size() && curr_pos.y + y_ofs < walk_map->size() && curr_pos.x + x_ofs >= 0 &&
        curr_pos.x + y_ofs >= 0)
    {
        if ((*walk_map)[curr_pos.x + x_ofs][curr_pos.y + y_ofs] == '.')
        {
            (*walk_map)[curr_pos.x + x_ofs][curr_pos.y + y_ofs] = 'O';
            temp_pos.x = curr_pos.x + x_ofs;
            temp_pos.y = curr_pos.y + y_ofs;
            all_positions->emplace_back(temp_pos, (*all_positions)[curr_index].second - 1);
        }
    }
}

void part2(long long *total, std::vector<std::vector<char>> walk_map, coords start, int steps)
{
    long long mid_grid = 0, pointy_tiles = 0, diagonal_tiles = 0, height = 404601, small_diag_steps, large_diag_steps,
              pointy_steps, debug_temp_val;
    std::vector<std::vector<char>> walk_map_temp;
    std::vector<std::pair<coords, int>> all_positions;

    // main full grid tiles
    //  origin type reacheable plots
    debug_temp_val = (count_by_pos_and_steps(231, 65, 65, walk_map));
    mid_grid += debug_temp_val * (((height - 3) * (height - 3) / 4));

    // origin offset 1 type reacheable plots
    debug_temp_val = (count_by_pos_and_steps(230, 65, 65, walk_map));
    mid_grid += debug_temp_val * (((height - 1) * (height - 1) / 4));

    // top, bottom, left, right end tiles
    pointy_steps = 130;
    //  uppermost tile plots
    debug_temp_val = count_by_pos_and_steps(pointy_steps, 130, 65, walk_map);
    pointy_tiles += debug_temp_val;

    // lowermost tile plots
    debug_temp_val = count_by_pos_and_steps(pointy_steps, 0, 65, walk_map);
    pointy_tiles += debug_temp_val;

    // leftmost tile plots
    debug_temp_val = count_by_pos_and_steps(pointy_steps, 65, 130, walk_map);
    pointy_tiles += debug_temp_val;

    // rightmost tile plots
    debug_temp_val = count_by_pos_and_steps(pointy_steps, 65, 0, walk_map);
    pointy_tiles += debug_temp_val;

    // partial tiles from diagonals
    small_diag_steps = 64;
    large_diag_steps = 195;
    // left-up diagonal plots
    debug_temp_val = count_by_pos_and_steps(small_diag_steps, 130, 130, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 1) / 2);
    debug_temp_val = count_by_pos_and_steps(large_diag_steps, 130, 130, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 3) / 2);

    // left-low diagonal plots
    debug_temp_val = count_by_pos_and_steps(small_diag_steps, 0, 130, walk_map);
    diagonal_tiles += (debug_temp_val + 15) * ((height - 1) / 2);
    debug_temp_val = count_by_pos_and_steps(large_diag_steps, 0, 130, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 3) / 2);

    // right-up diagonal plots
    debug_temp_val = count_by_pos_and_steps(small_diag_steps, 130, 0, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 1) / 2);
    debug_temp_val = count_by_pos_and_steps(large_diag_steps, 130, 0, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 3) / 2);

    // right-low diagonal plots
    debug_temp_val = count_by_pos_and_steps(small_diag_steps, 0, 0, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 1) / 2);
    debug_temp_val = count_by_pos_and_steps(large_diag_steps, 0, 0, walk_map);
    diagonal_tiles += debug_temp_val * ((height - 3) / 2);

    *total = (mid_grid + pointy_tiles + diagonal_tiles);
}

int count_by_pos_and_steps(int steps, int x, int y, std::vector<std::vector<char>> walk_map)
{
    int count = 0;
    coords start_point;
    std::vector<std::pair<coords, int>> all_positions;

    start_point.x = x;
    start_point.y = y;
    all_positions.emplace_back(start_point, steps);
    find_all_positions(&count, &all_positions, &walk_map, 0);

    return count;
}

int cout_not_rocks(std::vector<std::vector<char>> *walk_map)
{
    int count = 0;
    for (auto &&line : *walk_map)
    {
        for (auto &&plot : line)
        {
            if (plot == 'O')
            {
                count++;
            }
        }
    }

    return count;
}