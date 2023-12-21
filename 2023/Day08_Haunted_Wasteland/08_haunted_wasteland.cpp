#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>
#include <vector>

TimerUtility program_timer;
// time to solve: 6.9237ms

struct node_data
{
    char char1, char2, char3;
    int hash;
    int left;
    int right;
    bool end_A = false;
    bool end_Z = false;
};

void get_input(std::ifstream *, node_data *);
void get_part2_input(node_data *, int, std::vector<node_data> *);
int step_over(char, int, node_data *);
int get_all_ends_in_Z(int, std::string, node_data *);
int hash_func(std::string);
char get_next_step(std::string, int *);
bool check_loop(int, int);
long long get_lcm(std::vector<int>);

int main()
{

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    std::string instruction;
    input >> instruction;

    int counter = 0, instruction_step = 0, start_pos = hash_func("AAA"), end_pos = hash_func("ZZZ"),
        current_pos = start_pos;
    char next_step;
    node_data *node_list;
    node_list = new node_data[end_pos];
    std::vector<node_data> start_nodes;

    get_input(&input, node_list);
    get_part2_input(node_list, end_pos, &start_nodes);

    while (current_pos != end_pos)
    {
        next_step = get_next_step(instruction, &instruction_step);
        current_pos = step_over(next_step, current_pos, node_list);
        counter++;
    }

    std::cout << counter << std::endl;

    std::vector<int> ends_in_Z;

    for (int i = 0; i < start_nodes.size(); i++)
    {
        ends_in_Z.push_back(get_all_ends_in_Z(start_nodes[i].hash, instruction, node_list));
    }

    std::cout << get_lcm(ends_in_Z) << std::endl;

    program_timer.getDuration();

    return 0;
}

long long get_lcm(std::vector<int> ends_in_Z)
{
    long long lcm = 1;
    for (int i = 0; i < ends_in_Z.size(); i++)
    {
        lcm = std::lcm(lcm, ends_in_Z[i]);
    }

    return lcm;
}

int step_over(char step, int current_pos, node_data *node_list)
{
    if (step == 'L')
    {
        return node_list[current_pos].left;
    }
    else
    {
        return node_list[current_pos].right;
    }
}

void get_input(std::ifstream *input, node_data *node_list)
{

    int pos, left, right;
    std::string word;

    while ((*input) >> word)
    {
        pos = hash_func(word);
        node_list[pos].char1 = word[0];
        node_list[pos].char2 = word[1];
        node_list[pos].char3 = word[2];
        if (word[2] == 'A')
        {
            node_list[pos].end_A = true;
        }
        if (word[2] == 'Z')
        {
            node_list[pos].end_Z = true;
        }
        (*input) >> word;
        (*input) >> word;
        left = hash_func(word.substr(1, 3));
        (*input) >> word;
        right = hash_func(word.substr(0, 3));
        node_list[pos].hash = pos;
        node_list[pos].left = left;
        node_list[pos].right = right;
    }

    return;
}

void get_part2_input(node_data *node_list, int node_list_size, std::vector<node_data> *start_nodes)
{

    for (int i = 0; i < node_list_size; i++)
    {
        if (node_list[i].end_A)
        {
            (*start_nodes).push_back(node_list[i]);
        }
    }

    return;
}

int get_all_ends_in_Z(int current_node, std::string instruction, node_data *node_list)
{

    int instruction_step = 0, counter = 0;
    char next_step;
    int start_node = current_node;
    next_step = get_next_step(instruction, &instruction_step);
    current_node = step_over(next_step, current_node, node_list);
    counter++;
    std::vector<int> ends_in_Z_pos;

    if (node_list[current_node].end_Z)
    {
        ends_in_Z_pos.push_back(counter);
    }
    while (current_node != start_node)
    {

        next_step = get_next_step(instruction, &instruction_step);
        if (check_loop(current_node, step_over(next_step, current_node, node_list)))
        {
            break;
        }
        current_node = step_over(next_step, current_node, node_list);
        counter++;
        if (node_list[current_node].end_Z)
        {
            return counter;
        }
    }
    return 1;
}

bool check_loop(int current_pos, int next_pos)
{
    if (current_pos != next_pos)
    {
        return false;
    }
    else
    {
        return true;
    }
}

char get_next_step(std::string instructions, int *instruction_step)
{

    char next_step;

    (*instruction_step) %= instructions.length();

    next_step = instructions[*instruction_step];

    (*instruction_step)++;

    return next_step;
}

int hash_func(std::string node)
{

    int hash_value = 0;
    int alphabet_length = 'Z' - 'A' + 1;

    for (int i = 0; i < node.length(); i++)
    {
        hash_value += pow(alphabet_length, node.length() - i) * (node[i] - ('A' - 1));
    }

    return hash_value;
}