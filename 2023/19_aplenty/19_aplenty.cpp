#include "../../other/include_everything.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

TimerUtility input_timer, part1_timer, part2_timer;
// 1.1155ms  : data in + parsing
// 0.3597ms  : part1
// 0.4556ms  : part2

struct rule
{
    bool has_rule = true;
    char cat;
    char gt_lt;
    int cmp_to;
    std::string go_to;
};

typedef std::array<std::pair<int, int>, 4> part_range;

void get_data(std::string &, std::map<std::string, int> *, std::vector<std::vector<rule>> *,
              std::vector<std::vector<int>> *);
void part1(long long *, std::map<std::string, int> *, std::vector<std::vector<rule>> *,
           std::vector<std::vector<int>> *);
void part2(long long *, std::map<std::string, int> *, std::vector<std::vector<rule>> *);
long long get_comb(part_range, std::map<std::string, int> *, std::vector<std::vector<rule>> *, std::string, int);
std::pair<part_range, part_range> check_rule(part_range, rule);
bool check_valid(part_range *);

int main()
{
    std::string file_name = __INPUT_PATH;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::map<std::string, int> id_index_arr;
    std::vector<std::vector<rule>> workflows;
    std::vector<std::vector<int>> parts;
    long long part1_total, part2_total;

    input_timer.startTimer();
    get_data(file_name, &id_index_arr, &workflows, &parts);
    input_timer.getDuration("data in + parsing");

    part1_timer.startTimer();
    part1(&part1_total, &id_index_arr, &workflows, &parts);
    part1_timer.getDuration("part1");

    part2_timer.startTimer();
    part2(&part2_total, &id_index_arr, &workflows);
    part2_timer.getDuration("part2");

    std::cout << part1_total << '\n' << part2_total << '\n';

    return 0;
}

void get_data(std::string &file_name, std::map<std::string, int> *id_index_arr,
              std::vector<std::vector<rule>> *workflows, std::vector<std::vector<int>> *parts)
{
    std::ifstream input;
    input.open(file_name);
    std::vector<rule> workflow;
    std::string temp_string;
    std::vector<int> part;
    std::pair<std::string, int> id_index;
    int part_nr;
    rule temp_rule;

    while (getline(input, temp_string))
    {
        if (temp_string == "")
        {
            break;
        }
        id_index.first = temp_string.substr(0, temp_string.find_first_of('{'));
        id_index.second = id_index_arr->size();
        id_index_arr->insert(id_index);
        temp_string = temp_string.substr(temp_string.find('{') + 1, temp_string.length() - temp_string.find('{') - 2);

        temp_rule.has_rule = false;
        temp_rule.go_to = temp_string.substr(temp_string.find_last_of(',') + 1);
        workflows->push_back(std::vector<rule>());
        (*workflows).back().push_back(temp_rule);
        temp_rule.has_rule = true;

        while (temp_string.find_last_of(',') != std::string::npos)
        {
            temp_string = temp_string.substr(0, temp_string.find_last_of(','));
            temp_rule.go_to = temp_string.substr(temp_string.find_last_of(':') + 1,
                                                 temp_string.length() - temp_string.find_last_of(':') - 1);
            temp_rule.gt_lt = temp_string[temp_string.find_last_of("><")];
            temp_rule.cat = temp_string[temp_string.find_last_of("><") - 1];
            temp_rule.cmp_to = std::stoi(temp_string.substr(temp_string.find_last_of("><") + 1));

            (*workflows).back().push_back(temp_rule);
        }
    }

    while (getline(input, temp_string))
    {
        parts->push_back(std::vector<int>());
        while (temp_string.find_first_of(",") != std::string::npos)
        {
            part_nr = std::stoi(temp_string.substr(temp_string.find_first_of("=") + 1));
            (*parts).back().push_back(part_nr);
            temp_string = temp_string.substr(temp_string.find_first_of(",") + 1);
        }
        part_nr = std::stoi(temp_string.substr(temp_string.find_first_of("=") + 1));
        (*parts).back().push_back(part_nr);
    }

    for (auto &&workflow : *workflows)
    {
        std::reverse(workflow.begin(), workflow.end());
    }
}

void out_data()
{
    std::ofstream output;
    output.open("./output.txt");
}

void part1(long long *part1_total, std::map<std::string, int> *id_index_arr, std::vector<std::vector<rule>> *workflows,
           std::vector<std::vector<int>> *parts)
{
    *part1_total = 0;
    std::string up_next = "in", next_up_next = "in";
    int workf_index;
    for (auto &&part : *parts)
    {
        next_up_next = "in";
        while (up_next != "R" && up_next != "A")
        {
            up_next = next_up_next;
            if (up_next == "R")
            {
                up_next = "in";
                break;
            }
            if (up_next == "A")
            {
                up_next = "in";
                for (auto &&cat : part)
                {
                    *part1_total += cat;
                }
                break;
            }
            workf_index = id_index_arr->find(up_next)->second;
            for (std::vector<rule>::iterator iter = (*workflows)[workf_index].begin();
                 iter != (*workflows)[workf_index].end() && next_up_next == up_next; iter++)
            {
                if ((*iter).has_rule)
                {
                    switch ((*iter).cat)
                    {
                    case 'x':
                        if ((*iter).gt_lt == '>' && part[0] > (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        if ((*iter).gt_lt == '<' && part[0] < (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        break;
                    case 'm':
                        if ((*iter).gt_lt == '>' && part[1] > (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        if ((*iter).gt_lt == '<' && part[1] < (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        break;
                    case 'a':
                        if ((*iter).gt_lt == '>' && part[2] > (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        if ((*iter).gt_lt == '<' && part[2] < (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        break;
                    case 's':
                        if ((*iter).gt_lt == '>' && part[3] > (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        if ((*iter).gt_lt == '<' && part[3] < (*iter).cmp_to)
                        {
                            next_up_next = (*iter).go_to;
                        }
                        break;
                    }
                }
                else
                {
                    next_up_next = (*iter).go_to;
                }
            }
        }
    }
}

void part2(long long *part2_total, std::map<std::string, int> *id_index_arr, std::vector<std::vector<rule>> *workflows)
{
    std::pair<int, int> range = std::make_pair(1, 4000);
    *part2_total = get_comb({range, range, range, range}, id_index_arr, workflows, "in", 0);
}

long long get_comb(part_range range, std::map<std::string, int> *id_index_arr,
                   std::vector<std::vector<rule>> *workflows, std::string go_to, int index)
{
    long long total = 0;
    rule curr_rule;
    if (!check_valid(&range))
    {
        return 0;
    }
    if (go_to == "A")
    {
        total = 1;
        for (int i = 0; i < 4; i++)
        {
            total *= (range[i].second - range[i].first + 1);
        }
        return total;
    }
    if (go_to == "R")
    {
        return 0;
    }
    curr_rule = (*workflows)[id_index_arr->find(go_to)->second][index];
    if (curr_rule.has_rule)
    {
        std::pair<part_range, part_range> left_right_range;
        left_right_range = check_rule(range, curr_rule);
        if (curr_rule.gt_lt == '>')
        {
            total += get_comb(left_right_range.second, id_index_arr, workflows, curr_rule.go_to, 0);
            total += get_comb(left_right_range.first, id_index_arr, workflows, go_to, index + 1);
        }
        if (curr_rule.gt_lt == '<')
        {
            total += get_comb(left_right_range.first, id_index_arr, workflows, curr_rule.go_to, 0);
            total += get_comb(left_right_range.second, id_index_arr, workflows, go_to, index + 1);
        }
    }
    else
    {
        total += get_comb(range, id_index_arr, workflows, curr_rule.go_to, 0);
    }

    return total;
}

std::pair<part_range, part_range> check_rule(part_range range, rule to_check)
{
    part_range left_range = range, right_range = range;
    std::pair<part_range, part_range> left_right_range = std::make_pair(left_range, right_range);
    switch (to_check.cat)
    {
    case 'x':
        if (to_check.gt_lt == '>' && range[0].first < to_check.cmp_to)
        {
            left_right_range.first[0].second = to_check.cmp_to;
            left_right_range.second[0].first = to_check.cmp_to + 1;
            return left_right_range;
        }
        if (to_check.gt_lt == '<' && range[0].second > to_check.cmp_to)
        {
            left_right_range.first[0].second = to_check.cmp_to - 1;
            left_right_range.second[0].first = to_check.cmp_to;
            return left_right_range;
        }
        break;
    case 'm':
        if (to_check.gt_lt == '>' && range[1].first < to_check.cmp_to)
        {
            left_right_range.first[1].second = to_check.cmp_to;
            left_right_range.second[1].first = to_check.cmp_to + 1;
            return left_right_range;
        }
        if (to_check.gt_lt == '<' && range[1].second > to_check.cmp_to)
        {
            left_right_range.first[1].second = to_check.cmp_to - 1;
            left_right_range.second[1].first = to_check.cmp_to;
            return left_right_range;
        }
        break;
    case 'a':
        if (to_check.gt_lt == '>' && range[2].first < to_check.cmp_to)
        {
            left_right_range.first[2].second = to_check.cmp_to;
            left_right_range.second[2].first = to_check.cmp_to + 1;
            return left_right_range;
        }
        if (to_check.gt_lt == '<' && range[2].second > to_check.cmp_to)
        {
            left_right_range.first[2].second = to_check.cmp_to - 1;
            left_right_range.second[2].first = to_check.cmp_to;
            return left_right_range;
        }
        break;
    case 's':
        if (to_check.gt_lt == '>' && range[3].first < to_check.cmp_to)
        {
            left_right_range.first[3].second = to_check.cmp_to;
            left_right_range.second[3].first = to_check.cmp_to + 1;
            return left_right_range;
        }
        if (to_check.gt_lt == '<' && range[3].second > to_check.cmp_to)
        {
            left_right_range.first[3].second = to_check.cmp_to - 1;
            left_right_range.second[3].first = to_check.cmp_to;
            return left_right_range;
        }
        break;
    }
    return left_right_range;
}

bool check_valid(part_range *range)
{
    for (auto &&categ : *range)
    {
        if (categ.second < categ.first)
        {
            return false;
        }
    }
    return true;
}