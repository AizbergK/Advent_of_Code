#include "../../other/include_everything.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility program_timer;
// time to solve: 4.8986ms

struct coord
{
    int x;
    int y;
    int x_exp_coef = 0;
    int y_exp_coef = 0;
};

char **get_cosmos(std::string, int *, int *);
void print_cosmos(char **);
void expand_cosmos(char **, int *, int *);
void get_stars(std::vector<coord> *, char **);
void get_star_pairs(std::vector<std::pair<coord *, coord *>> *, std::vector<coord> *);
void get_exp_coef(std::vector<coord> *, std::vector<coord> *);
long long get_distance(coord *, coord *, int);
long long get_distance_total(std::vector<std::pair<coord *, coord *>> *, int);

int main()
{

    program_timer.startTimer();

    std::string file_name = __INPUT_PATH;
    char **cosmos;
    int line_number, col_number, expansion_value = 1'000'000;
    std::vector<coord> stars, stars_after_expansion;
    std::vector<std::pair<coord *, coord *>> star_pairs;

    cosmos = get_cosmos(file_name, &line_number, &col_number);
    get_stars(&stars, cosmos);
    expand_cosmos(cosmos, &line_number, &col_number);
    get_stars(&stars_after_expansion, cosmos);
    get_star_pairs(&star_pairs, &stars);
    get_exp_coef(&stars, &stars_after_expansion);

    std::cout << get_distance_total(&star_pairs, 2) << std::endl;
    std::cout << get_distance_total(&star_pairs, expansion_value) << std::endl;
    // print_cosmos(cosmos);

    program_timer.getDuration();

    return 0;
}

char **get_cosmos(std::string file_name, int *line_number, int *col_number)
{

    std::ifstream input;
    input.open(file_name);
    std::string line;
    *line_number = 1;

    std::getline(input, line);
    *col_number = line.size();

    while (std::getline(input, line))
    {
        (*line_number)++;
    }

    input.close();
    input.open(file_name);

    char **cosmos = new char *[(*line_number) * 2];
    for (int i = 0; i < (*line_number); i++)
    {
        cosmos[i] = new char[(*col_number) * 2];
        std::getline(input, line);
        for (int j = 0; j < (*col_number); j++)
        {
            cosmos[i][j] = line[j];
        }
        cosmos[i][(*col_number)] = '\0';
    }
    for (int i = (*line_number); i < (*line_number) * 2; i++)
    {
        cosmos[i] = new char[(*col_number) * 2];
    }
    cosmos[(*line_number)] = NULL;

    return cosmos;
}

void print_cosmos(char **cosmos)
{

    std::ofstream output;
    output.open("./output.txt");

    for (int i = 0; cosmos[i] != NULL; i++)
    {
        for (int j = 0; cosmos[i][j] != '\0'; j++)
        {
            output << cosmos[i][j];
        }
        output << '\n';
    }

    output.close();

    return;
}

void expand_cosmos(char **cosmos, int *line_number, int *col_number)
{

    bool is_line_empty;

    for (int i = 0; i < *line_number; i++)
    {
        is_line_empty = true;
        for (int j = 0; j < *col_number; j++)
        {
            if (cosmos[i][j] == '#')
            {
                is_line_empty = false;
                break;
            }
        }
        if (is_line_empty)
        {
            for (int k = *line_number; k >= i; k--)
            {
                cosmos[k + 1] = cosmos[k];
            }
            i++;
            (*line_number)++;
        }
    }

    for (int i = 0; i < *col_number; i++)
    {
        is_line_empty = true;
        for (int j = 0; j < *line_number; j++)
        {
            if (cosmos[j][i] == '#')
            {
                is_line_empty = false;
                break;
            }
        }
        if (is_line_empty)
        {
            for (int k = 0; k < *line_number; k++)
            {
                memmove(cosmos[k] + i + 1, cosmos[k] + i, sizeof(char) * (*col_number - i));
                cosmos[k][*col_number + 1] = '\0';
            }
            i++;
            (*col_number)++;
        }
    }

    return;
}

void get_stars(std::vector<coord> *stars, char **cosmos)
{

    coord star;

    for (int i = 0; cosmos[i] != NULL; i++)
    {
        for (int j = 0; cosmos[i][j] != '\0'; j++)
        {
            if (cosmos[i][j] == '#')
            {
                star.x = i;
                star.y = j;
                (*stars).push_back(star);
            }
        }
    }

    return;
}

void get_star_pairs(std::vector<std::pair<coord *, coord *>> *star_pairs, std::vector<coord> *stars)
{

    std::pair<coord *, coord *> star_pair;

    for (int i = 0; i < stars->size(); i++)
    {
        for (int j = i + 1; j < stars->size(); j++)
        {
            star_pair.first = &((*stars)[i]);
            star_pair.second = &((*stars)[j]);
            (*star_pairs).push_back(star_pair);
        }
    }

    return;
}

void get_exp_coef(std::vector<coord> *stars, std::vector<coord> *stars_after_expansion)
{

    for (int i = 0; i < stars->size(); i++)
    {
        (*stars)[i].x_exp_coef = (*stars_after_expansion)[i].x - (*stars)[i].x;
        (*stars)[i].y_exp_coef = (*stars_after_expansion)[i].y - (*stars)[i].y;
    }

    return;
}

long long get_distance(coord *first_star, coord *second_star, int expansion_value)
{

    long long distance = 0;
    distance += std::abs((second_star->x + second_star->x_exp_coef * expansion_value) -
                         (first_star->x + first_star->x_exp_coef * expansion_value));
    distance += std::abs((second_star->y + second_star->y_exp_coef * expansion_value) -
                         (first_star->y + first_star->y_exp_coef * expansion_value));

    return distance;
}

long long get_distance_total(std::vector<std::pair<coord *, coord *>> *star_pairs, int expansion_value)
{

    long long distance_total = 0;

    for (int i = 0; i < star_pairs->size(); i++)
    {
        distance_total += get_distance((*star_pairs)[i].first, (*star_pairs)[i].second, expansion_value - 1);
    }

    return distance_total;
}
