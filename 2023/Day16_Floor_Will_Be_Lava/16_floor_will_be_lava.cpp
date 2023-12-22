#include "../../timer_utility.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>

TimerUtility program_timer;
// time to solve: 276.38ms

struct grid_el
{
    char symb;
    bool no_energy = true;
    bool was_split = false;
};

struct path_trace
{
    int x;
    int y;
    char to;
};

void get_data_and_init(std::vector<std::vector<grid_el>> *, std::string);
void energize_grid(int *, std::vector<std::vector<grid_el>>, int, int, char);
bool check_bounds(int, int, path_trace);
void mirror_splitter(grid_el *, int, std::vector<path_trace> *);
void step(path_trace *);

int main()
{
    program_timer.startTimer();

    std::string file_name = "input.txt";
    std::ofstream output;
    output.open("./output.txt");
    int first_val = 0, max_val = 0;

    std::vector<std::vector<grid_el>> grid;
    std::vector<path_trace> all_paths;
    std::vector<std::thread> all_threads;

    get_data_and_init(&grid, file_name);
    all_threads.push_back(std::thread(energize_grid, &first_val, grid, 0, 0, 'r'));

    for (int i = 0; i < grid.size(); i++)
    {
        all_threads.push_back(std::thread(&energize_grid, &max_val, grid, i, 0, 'r'));
        all_threads.push_back(std::thread(&energize_grid, &max_val, grid, i, grid[0].size() - 1, 'l'));
    }
    for (int i = 0; i < grid[0].size(); i++)
    {
        all_threads.push_back(std::thread(&energize_grid, &max_val, grid, 0, i, 'd'));
        all_threads.push_back(std::thread(&energize_grid, &max_val, grid, grid.size() - 1, i, 'u'));
    }

    for (int i = 0; i < all_threads.size(); i++)
    {
        all_threads[i].detach();
    }

    std::cout << first_val << std::endl << max_val << std::endl;

    program_timer.getDuration();

    return 0;
}

void get_data_and_init(std::vector<std::vector<grid_el>> *grid, std::string file_name)
{
    grid_el elem;
    std::string line;
    std::ifstream input;
    input.open(file_name);

    for (int i = 0; std::getline(input, line); i++)
    {
        grid->resize(grid->size() + 1);
        for (int j = 0; j < line.length(); j++)
        {
            elem.symb = line[j];
            (*grid)[i].push_back(elem);
        }
    }
}

void energize_grid(int *max_val, std::vector<std::vector<grid_el>> grid, int x, int y, char dir)
{
    const int r_boud = grid[0].size(), d_bound = grid.size();
    int counter = 0;
    std::vector<path_trace> all_paths;
    path_trace start;
    start.to = dir;
    start.x = x;
    start.y = y;
    all_paths.push_back(start);

    for (int i = 0; i < all_paths.size(); i++)
    {
        while (check_bounds(r_boud, d_bound, all_paths[i]))
        {
            if (grid[all_paths[i].x][all_paths[i].y].no_energy)
            {
                grid[all_paths[i].x][all_paths[i].y].no_energy = false;
                counter++;
            }
            mirror_splitter(&(grid[all_paths[i].x][all_paths[i].y]), i, &all_paths);
            step(&(all_paths[i]));
        }
    }

    (*max_val) = std::max((*max_val), counter);
}

bool check_bounds(int r_boud, int d_bound, path_trace curr_path)
{

    if (curr_path.x < 0)
    {
        return false;
    }
    if (curr_path.x == r_boud)
    {
        return false;
    }
    if (curr_path.y < 0)
    {
        return false;
    }
    if (curr_path.y == d_bound)
    {
        return false;
    }

    return true;
}

void step(path_trace *path)
{
    switch ((*path).to)
    {
    case 'u':
        (*path).x--;
        break;
    case 'd':
        (*path).x++;
        break;
    case 'r':
        (*path).y++;
        break;
    case 'l':
        (*path).y--;
        break;
    }
}

void mirror_splitter(grid_el *el, int index, std::vector<path_trace> *all_paths)
{
    switch ((*all_paths)[index].to)
    {
    case 'r':
        switch ((*el).symb)
        {
        case '/':
            (*all_paths)[index].to = 'u';
            break;

        case '\\':
            (*all_paths)[index].to = 'd';
            break;
        case '|':
            if (!(*el).was_split)
            {
                (*el).was_split = true;
                path_trace path1 = (*all_paths)[index], path2 = (*all_paths)[index];
                path1.to = 'u';
                path2.to = 'd';
                (*all_paths).push_back(path1);
                (*all_paths).push_back(path2);
            }
            (*all_paths)[index].x = -2;
            break;
        }
        break;
    case 'l':
        switch ((*el).symb)
        {
        case '/':
            (*all_paths)[index].to = 'd';
            break;

        case '\\':
            (*all_paths)[index].to = 'u';
            break;
        case '|':
            if (!(*el).was_split)
            {
                (*el).was_split = true;
                path_trace path1 = (*all_paths)[index], path2 = (*all_paths)[index];
                path1.to = 'u';
                path2.to = 'd';
                (*all_paths).push_back(path1);
                (*all_paths).push_back(path2);
            }
            (*all_paths)[index].x = -2;
            break;
        }
        break;
    case 'u':
        switch ((*el).symb)
        {
        case '/':
            (*all_paths)[index].to = 'r';
            break;

        case '\\':
            (*all_paths)[index].to = 'l';
            break;
        case '-':
            if (!(*el).was_split)
            {
                (*el).was_split = true;
                path_trace path1 = (*all_paths)[index], path2 = (*all_paths)[index];
                path1.to = 'l';
                path2.to = 'r';
                (*all_paths).push_back(path1);
                (*all_paths).push_back(path2);
            }
            (*all_paths)[index].x = -2;
        }
        break;
    case 'd':
        switch ((*el).symb)
        {
        case '/':
            (*all_paths)[index].to = 'l';
            break;

        case '\\':
            (*all_paths)[index].to = 'r';
            break;
        case '-':
            if (!(*el).was_split)
            {
                (*el).was_split = true;
                path_trace path1 = (*all_paths)[index], path2 = (*all_paths)[index];
                path1.to = 'l';
                path2.to = 'r';
                (*all_paths).push_back(path1);
                (*all_paths).push_back(path2);
            }
            (*all_paths)[index].x = -2;
            break;
        }
        break;
    }
}