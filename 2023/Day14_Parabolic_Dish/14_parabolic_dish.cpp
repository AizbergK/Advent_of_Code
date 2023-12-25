#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility program_timer;
// time to solve: 77.701ms

typedef std::vector<std::string> matrix;

void tilt_north(matrix *);
int count_load(matrix *);
void move_rock_north(std::string *, int);
int count_col_load(std::string *);
void spin_cycle(matrix *);
void rotate_dish(matrix *);
void flip_vertical(matrix *);
void rotate_dish_one_shot(matrix *);
void flip_vertical_row(std::string *);
bool compare_matrix(matrix *, matrix *);
int compare_matrix_back_to_front(std::vector<matrix> *, matrix *, int *);

int main()
{
    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");

    std::string line, char_string;
    matrix the_dish, temp_dish;
    std::vector<matrix> the_dish_permutations;
    bool keep_going = true;
    int repeating_index, period, last_index;

    input >> line;
    for (int i = line.length() - 1; i >= 0; i--)
    {
        char_string = line[i];
        the_dish.push_back(char_string);
    }
    while (!input.eof())
    {
        input >> line;
        for (int i = 0; i < line.length(); i++)
        {
            the_dish[i] += line[line.length() - i - 1];
        }
    }

    tilt_north(&the_dish);
    std::cout << count_load(&the_dish) << std::endl;

    for (int i = 0; i < 4; i++)
    {

        tilt_north(&the_dish);
        rotate_dish(&the_dish);
    }
    the_dish_permutations.push_back(the_dish);
    while (keep_going)
    {
        temp_dish = the_dish_permutations.back();
        spin_cycle(&temp_dish);
        repeating_index = compare_matrix_back_to_front(&the_dish_permutations, &temp_dish, &repeating_index);
        if (repeating_index)
        {
            repeating_index -= 1;
            period = the_dish_permutations.size() - repeating_index;
            keep_going = false;
        }
        else
        {
            the_dish_permutations.push_back(temp_dish);
            temp_dish.clear();
        }
    }

    last_index = repeating_index + (1'000'000'000 - repeating_index) % period - 1;

    std::cout << count_load(&the_dish_permutations[last_index]) << std::endl;

    program_timer.getDuration();

    return 0;
}

void tilt_north(matrix *the_dish)
{

    for (int i = 0; i < the_dish->size(); i++)
    {
        for (int j = 1; j < (*the_dish)[0].length(); j++)
        {
            if ((*the_dish)[i][j - 1] == '.' && (*the_dish)[i][j] == 'O')
            {
                move_rock_north(&(*the_dish)[i], j);
            }
        }
    }

    return;
}

void move_rock_north(std::string *dish_line, int from)
{

    (*dish_line)[from] = '.';
    for (int i = from - 1; i >= 0; i--)
    {
        if ((*dish_line)[i] == 'O' || (*dish_line)[i] == '#')
        {
            (*dish_line)[i + 1] = 'O';
            return;
        }
    }
    if ((*dish_line)[0] == '.')
    {
        (*dish_line)[0] = 'O';
    }

    return;
}

int count_load(matrix *the_dish)
{

    int load = 0;

    for (int i = 0; i < the_dish->size(); i++)
    {
        load += count_col_load(&(*the_dish)[i]);
    }

    return load;
}

int count_col_load(std::string *dish_col)
{

    int col_load = 0, height = (*dish_col).length();

    for (int i = 0; i < height; i++)
    {
        if ((*dish_col)[i] == 'O')
        {
            col_load += height - i;
        }
    }

    return col_load;
}

void spin_cycle(matrix *the_dish)
{

    tilt_north(the_dish);

    for (int i = 0; i < 3; i++)
    {

        rotate_dish(the_dish);
        tilt_north(the_dish);
    }

    rotate_dish(the_dish);

    return;
}

void rotate_dish(matrix *the_dish)
{

    int height = the_dish->size();
    char temp;

    for (int i = 0; i < height / 2; i++)
    {
        for (int j = i; j < height - i - 1; j++)
        {
            temp = (*the_dish)[i][j];
            (*the_dish)[i][j] = (*the_dish)[height - j - 1][i];
            (*the_dish)[height - j - 1][i] = (*the_dish)[height - i - 1][height - j - 1];
            (*the_dish)[height - i - 1][height - j - 1] = (*the_dish)[j][height - i - 1];
            (*the_dish)[j][height - i - 1] = temp;
        }
    }

    return;
}

int compare_matrix_back_to_front(std::vector<matrix> *all_matrix, matrix *second_matrix, int *index)
{

    for (std::vector<matrix>::reverse_iterator i = (*all_matrix).rbegin(); i != (*all_matrix).rend(); i++)
    {
        if (compare_matrix(&(*i), second_matrix))
        {
            return -std::distance((*all_matrix).rend(), i);
        }
    }

    return 0;
}

bool compare_matrix(matrix *first_matrix, matrix *second_matrix)
{

    for (int i = 0; i < first_matrix->size(); i++)
    {
        if (((*first_matrix)[i]).compare((*second_matrix)[i]))
        {
            return false;
        }
    }

    return true;
}