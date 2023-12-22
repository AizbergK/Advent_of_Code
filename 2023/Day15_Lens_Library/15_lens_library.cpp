#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <vector>

TimerUtility program_timer;
// time to solve: 1.1238ms

std::pair<std::string, int> get_lens(std::string);
int hash_instr(std::string);
void add_lens(std::pair<std::string, int>, std::vector<std::vector<std::pair<std::string, int>>> *, int);
void remove_lens(std::pair<std::string, int>, std::vector<std::vector<std::pair<std::string, int>>> *, int);
int get_focusing_power(std::vector<std::vector<std::pair<std::string, int>>> *);
int get_hash_total(std::string *);
void fill_boxes(std::string *, std::vector<std::vector<std::pair<std::string, int>>> *);

int main()
{
    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    int hash_total, total_focusing_power;
    std::vector<std::vector<std::pair<std::string, int>>> boxes;
    std::string instr_seq;
    boxes.resize(256);
    std::getline(input, instr_seq);

    // part 1
    hash_total = get_hash_total(&instr_seq);
    std::cout << hash_total << '\n';

    // part 2
    fill_boxes(&instr_seq, &boxes);
    total_focusing_power = get_focusing_power(&boxes);
    std::cout << total_focusing_power << '\n';

    program_timer.getDuration();

    return 0;
}

int hash_instr(std::string instr)
{
    int hash = 0;
    for (int i = 0; i < instr.length(); i++)
    {
        hash += instr[i];
        hash *= 17;
        hash %= 256;
    }

    return hash;
}

int get_hash_total(std::string *full_instr)
{
    int hash_total = 0;
    std::string instr;

    for (int i = 0; i < (*full_instr).length(); i++)
    {
        if ((*full_instr)[i] == ',')
        {
            hash_total += hash_instr(instr);
            instr.clear();
        }
        else
        {
            instr += (*full_instr)[i];
        }
    }
    hash_total += hash_instr(instr);

    return hash_total;
}

std::pair<std::string, int> get_lens(std::string instr)
{
    std::pair<std::string, int> hash_focal;
    int instr_pos;

    instr_pos = instr.find_first_of("-=");
    hash_focal.first = hash_instr(instr.substr(0, instr_pos - 1));
    hash_focal.second = instr[instr_pos + 1] - '0';

    return hash_focal;
}

void add_lens(std::pair<std::string, int> hash_focal, std::vector<std::vector<std::pair<std::string, int>>> *boxes,
              int box_index)
{
    for (int i = 0; i < (*boxes)[box_index].size(); i++)
    {
        if ((*boxes)[box_index][i].first == hash_focal.first)
        {
            (*boxes)[box_index][i] = hash_focal;
            return;
        }
    }

    (*boxes)[box_index].push_back(hash_focal);

    return;
}

void remove_lens(std::pair<std::string, int> hash_focal, std::vector<std::vector<std::pair<std::string, int>>> *boxes,
                 int box_index)
{
    for (int i = 0; i < (*boxes)[box_index].size(); i++)
    {
        if ((*boxes)[box_index][i].first == hash_focal.first)
        {
            (*boxes)[box_index].erase((*boxes)[box_index].begin() + i);
            return;
        }
    }

    return;
}

int get_focusing_power(std::vector<std::vector<std::pair<std::string, int>>> *boxes)
{
    int focusing_power = 0;

    for (int i = 0; i < boxes->size(); i++)
    {
        for (int j = 0; j < (*boxes)[i].size(); j++)
        {
            focusing_power += (i + 1) * (j + 1) * (*boxes)[i][j].second;
        }
    }

    return focusing_power;
}

void fill_boxes(std::string *full_instr, std::vector<std::vector<std::pair<std::string, int>>> *boxes)
{
    int hash;
    std::string instr;
    std::pair<std::string, int> id_focal;

    for (int i = 0; i < (*full_instr).length(); i++)
    {
        if (isalpha((*full_instr)[i]))
        {
            instr += (*full_instr)[i];
        }
        if ((*full_instr)[i] == ',')
        {
            instr.clear();
        }
        if ((*full_instr)[i] == '=')
        {
            id_focal.first = instr;
            id_focal.second = (*full_instr)[i + 1] - '0';
            hash = hash_instr(instr);
            add_lens(id_focal, boxes, hash);
        }
        if ((*full_instr)[i] == '-')
        {
            id_focal.first = instr;
            hash = hash_instr(instr);
            remove_lens(id_focal, boxes, hash);
        }
    }

    return;
}