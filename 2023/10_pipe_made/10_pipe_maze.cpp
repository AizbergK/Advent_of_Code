#include "../../other/include_everything.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

TimerUtility total_timer;
// time to solve: 12.4384ms

struct pipe
{
    bool north = false;
    bool south = false;
    bool west = false;
    bool east = false;
};

struct dimensions
{
    int width;
    int height;
};

int run_maze(dimensions, std::vector<std::vector<pipe>> *, std::vector<std::vector<bool>> *);
void advance(dimensions *, pipe *, std::vector<std::vector<pipe>> *);
pipe define_start_pipe(dimensions, std::vector<std::vector<pipe>> *);
pipe get_connection(char);
int count_contained_spaces(std::vector<std::vector<char>> *);
bool is_inside_heightwise(int, int, std::vector<std::vector<char>> *);
char get_start_pipe_char(pipe);
bool check_direction(char, char);

int main()
{

    total_timer.startTimer();

    std::string file_name = __INPUT_PATH;
    int maze_length, contained_spaces;
    char input_char, start_char;

    std::ifstream input;
    input.open(file_name);

    std::string line;

    std::getline(input, line);

    dimensions maze_dimensions, start_coord;

    maze_dimensions.width = line.length();
    maze_dimensions.height = 0;

    std::vector<std::vector<pipe>> maze;
    std::vector<pipe> maze_line;

    for (int i = 0; i < maze_dimensions.width; i++)
    {
        maze_line.push_back(get_connection(line[i]));
        if (line[i] == 'S')
        {
            start_coord.width = i;
            start_coord.height = maze_dimensions.height;
        }
    }

    maze_dimensions.height++;
    maze.push_back(maze_line);
    maze_line.clear();

    while (std::getline(input, line))
    {

        for (int i = 0; i < maze_dimensions.width; i++)
        {
            maze_line.push_back(get_connection(line[i]));
            if (line[i] == 'S')
            {
                start_coord.width = i;
                start_coord.height = maze_dimensions.height;
            }
        }

        maze_dimensions.height++;
        maze.push_back(maze_line);
        maze_line.clear();
    }

    input.close();

    std::vector<std::vector<bool>> clean_maze;
    std::vector<std::vector<char>> char_maze;

    clean_maze.resize(maze_dimensions.height);
    char_maze.resize(maze_dimensions.height);

    for (int i = 0; i < maze_dimensions.height; i++)
    {
        clean_maze[i].resize(maze_dimensions.width);
        char_maze[i].resize(maze_dimensions.width);
    }

    maze_length = run_maze(start_coord, &maze, &clean_maze);

    std::cout << maze_length << std::endl;

    input.open(file_name);

    start_char = get_start_pipe_char(define_start_pipe(start_coord, &maze));

    for (int i = 0; i < maze_dimensions.height; i++)
    {
        for (int j = 0; j < maze_dimensions.width; j++)
        {
            input >> input_char;
            if (clean_maze[i][j])
            {
                if (input_char != 'S')
                {
                    char_maze[i][j] = input_char;
                }
                else
                {
                    char_maze[i][j] = start_char;
                }
            }
            else
            {
                char_maze[i][j] = '.';
            }
        }
    }

    input.close();

    contained_spaces = count_contained_spaces(&char_maze);

    std::cout << contained_spaces << std::endl;

    total_timer.getDuration();

    return 0;
}

int run_maze(dimensions start, std::vector<std::vector<pipe>> *maze, std::vector<std::vector<bool>> *clean_maze)
{

    int counter = 1;
    pipe start_pipe;
    pipe both_sides[2];
    dimensions sides[2];
    start_pipe = define_start_pipe(start, maze);
    (*clean_maze)[start.height][start.width] = true;
    for (int i = 0; i < 2; i++)
    {
        if (start_pipe.east)
        {
            both_sides[i].west = true;
            sides[i].width = start.width + 1;
            sides[i].height = start.height;
            start_pipe.east = false;
            continue;
        }
        if (start_pipe.west)
        {
            both_sides[i].east = true;
            sides[i].width = start.width - 1;
            sides[i].height = start.height;
            start_pipe.west = false;
            continue;
        }
        if (start_pipe.north)
        {
            both_sides[i].south = true;
            sides[i].width = start.width;
            sides[i].height = start.height - 1;
            start_pipe.north = false;
            continue;
        }
        if (start_pipe.south)
        {
            both_sides[i].north = true;
            sides[i].width = start.width;
            sides[i].height = start.height + 1;
            start_pipe.south = false;
            continue;
        }
    }
    (*clean_maze)[sides[0].height][sides[0].width] = true;
    (*clean_maze)[sides[1].height][sides[1].width] = true;

    while (sides[0].height != sides[1].height || sides[0].width != sides[1].width)
    {

        for (int i = 0; i < 2; i++)
        {
            advance(&sides[i], &both_sides[i], maze);
            (*clean_maze)[sides[i].height][sides[i].width] = true;
        }
        counter++;
    }

    return counter;
}

void advance(dimensions *location, pipe *from, std::vector<std::vector<pipe>> *maze)
{
    int continue_case;

    if ((*maze)[(*location).height][(*location).width].east)
    {
        if (!(*from).east)
        {
            continue_case = 1;
        }
    }
    if ((*maze)[(*location).height][(*location).width].west)
    {
        if (!(*from).west)
        {
            continue_case = 2;
        }
    }
    if ((*maze)[(*location).height][(*location).width].north)
    {
        if (!(*from).north)
        {
            continue_case = 3;
        }
    }
    if ((*maze)[(*location).height][(*location).width].south)
    {
        if (!(*from).south)
        {
            continue_case = 4;
        }
    }

    switch (continue_case)
    {
    case 1:
        (*location).width = (*location).width + 1;
        (*from).east = false;
        (*from).west = true;
        (*from).north = false;
        (*from).south = false;
        break;
    case 2:
        (*location).width = (*location).width - 1;
        (*from).east = true;
        (*from).west = false;
        (*from).north = false;
        (*from).south = false;
        break;
    case 3:
        (*location).height = (*location).height - 1;
        (*from).east = false;
        (*from).west = false;
        (*from).north = false;
        (*from).south = true;
        break;
    case 4:
        (*location).height = (*location).height + 1;
        (*from).east = false;
        (*from).west = false;
        (*from).north = true;
        (*from).south = false;
        break;
    }

    return;
}

pipe define_start_pipe(dimensions start, std::vector<std::vector<pipe>> *maze)
{

    dimensions left, right, top, bot;
    pipe start_pipe;

    left.height = start.height;
    left.width = start.width - 1;

    right.height = start.height;
    right.width = start.width + 1;

    top.height = start.height - 1;
    top.width = start.width;

    bot.height = start.height + 1;
    bot.width = start.width;

    if ((*maze)[left.height][left.width].east && left.width >= 0)
    {
        start_pipe.west = true;
    }
    if ((*maze)[right.height][right.width].west && right.width < (*maze)[0].size())
    {
        start_pipe.east = true;
    }
    if (top.height >= 0 && (*maze)[top.height][top.width].south)
    {
        start_pipe.north = true;
    }
    if ((*maze)[bot.height][bot.width].north && bot.height < (*maze).size())
    {
        start_pipe.south = true;
    }

    return start_pipe;
}

char get_start_pipe_char(pipe start_pipe)
{

    pipe temp_pipe;
    char start_char;
    std::string pipe_chars = "|-LJ7F";

    for (int i = 0; i < pipe_chars.length(); i++)
    {
        temp_pipe = get_connection(pipe_chars[i]);
        if (temp_pipe.east == start_pipe.east && temp_pipe.west == start_pipe.west &&
            temp_pipe.north == start_pipe.north && temp_pipe.south == start_pipe.south)
        {
            start_char = pipe_chars[i];
            break;
        }
    }
    return start_char;
}

pipe get_connection(char letter)
{

    pipe connection;

    switch (letter)
    {
    case '|':
        connection.north = true;
        connection.south = true;
        break;

    case '-':
        connection.east = true;
        connection.west = true;
        break;
    case 'L':
        connection.north = true;
        connection.east = true;
        break;
    case 'J':
        connection.north = true;
        connection.west = true;
        break;
    case '7':
        connection.south = true;
        connection.west = true;
        break;
    case 'F':
        connection.east = true;
        connection.south = true;
        break;
    default:
        break;
    }

    return connection;
}

int count_contained_spaces(std::vector<std::vector<char>> *char_maze)
{

    int count = 0;
    bool is_inside_width, is_inside_height, is_on_line;
    char first_corner, second_corner;

    for (int i = 0; i < (*char_maze).size(); i++)
    {
        is_inside_width = false;
        is_on_line = false;
        for (int j = 0; j < (*char_maze)[i].size(); j++)
        {
            if ((*char_maze)[i][j] == '|')
            {
                is_inside_width = !is_inside_width;
            }
            if (((*char_maze)[i][j] == 'L' || (*char_maze)[i][j] == 'F') && !is_on_line)
            {
                first_corner = (*char_maze)[i][j];
                is_on_line = !is_on_line;
            }
            if (((*char_maze)[i][j] == 'J' || (*char_maze)[i][j] == '7') && is_on_line)
            {
                second_corner = (*char_maze)[i][j];
                is_on_line = !is_on_line;
                if (check_direction(first_corner, second_corner))
                {
                    is_inside_width = !is_inside_width;
                }
            }
            if (is_inside_width)
            {
                is_inside_height = is_inside_heightwise(i, j, char_maze);
                if (is_inside_height && is_inside_width && (*char_maze)[i][j] == '.')
                {
                    count++;
                }
            }
        }
    }

    return count;
}

bool is_inside_heightwise(int height, int width, std::vector<std::vector<char>> *char_maze)
{

    bool is_inside_height = false, is_on_line = false;
    char first_corner, second_corner;

    for (int i = 0; i <= height; i++)
    {
        if ((*char_maze)[i][width] == '-')
        {
            is_inside_height = !is_inside_height;
        }
        if (((*char_maze)[i][width] == '7' || (*char_maze)[i][width] == 'F') && !is_on_line)
        {
            first_corner = (*char_maze)[i][width];
            is_on_line = !is_on_line;
        }
        if (((*char_maze)[i][width] == 'J' || (*char_maze)[i][width] == 'L') && is_on_line)
        {
            second_corner = (*char_maze)[i][width];
            is_on_line = !is_on_line;
            if (check_direction(first_corner, second_corner))
            {
                is_inside_height = !is_inside_height;
            }
        }
    }

    return is_inside_height;
}

bool check_direction(char first_corner, char second_corner)
{

    bool is_opposite_direction;

    if (first_corner == '7' && second_corner == 'J')
    {
        is_opposite_direction = false;
    }
    else if (first_corner == '7' && second_corner == 'L')
    {
        is_opposite_direction = true;
    }
    if (first_corner == 'F' && second_corner == 'L')
    {
        is_opposite_direction = false;
    }
    else if (first_corner == 'F' && second_corner == 'J')
    {
        is_opposite_direction = true;
    }
    if (first_corner == 'L' && second_corner == 'J')
    {
        is_opposite_direction = false;
    }
    else if (first_corner == 'L' && second_corner == '7')
    {
        is_opposite_direction = true;
    }
    if (first_corner == 'F' && second_corner == '7')
    {
        is_opposite_direction = false;
    }
    else if (first_corner == 'F' && second_corner == 'J')
    {
        is_opposite_direction = true;
    }

    return is_opposite_direction;
}