#include<fstream>
#include<iostream>
#include<string.h>
#include<regex>
#include"../../timer_utility.h"

TimerUtility program_timer;
//time to solve: 0.927ms

bool game_check(std::string, int*, int*);
bool subset_check(std::string, int*, int*);

int main() {

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");

    int id_sum = 0, id_counter = 0, power_sum = 0, power;
    int cubes_max[3] = {12, 13, 14}; // red, green, blue

    std::string game;

    while(!input.eof()) {
        id_counter++;
        std::getline(input, game);
        if(game_check(game.substr((game.find(':')) + 2), cubes_max, &power)) {
            id_sum += id_counter;
        };
        power_sum += power;
    }

    std::cout << id_sum << " id sum" << std::endl;
    std::cout << power_sum << " power sum" << std::endl;

    program_timer.getDuration();

    return 0;
}

bool game_check(std::string game, int* cubes_max, int *power) {

    std::regex word("; || ,");
    int substring_start = 0;
    int cube_min_nr[3] = {0, 0, 0};
    bool is_possible = true;

    std::string substring = game.substr(0, game.find_first_of(";,"));
    while (game.find_first_of(";,", substring_start) != std::string::npos)
    {
        substring_start = game.find_first_of(";,", substring_start + 2);
        is_possible = subset_check(substring, cubes_max, cube_min_nr);
        substring = game.substr(substring_start + 2, game.find_first_of(";,", substring_start + 2) - substring_start);
    }

    *power = cube_min_nr[0] * cube_min_nr[1] * cube_min_nr[2];
    return is_possible;
}

bool subset_check(std::string substring, int* cubes_max, int* cubes_min_nr) {

    int cubes = std::stoi(substring);
    bool is_possible = true;

    if(substring.find("red") != std::string::npos) {
        if(cubes > cubes_max[0]) {
            is_possible = false;
        }
        if(cubes > cubes_min_nr[0]) {
            cubes_min_nr[0] = cubes;
        }
    }
        
    if(substring.find("green") != std::string::npos) {
        if(cubes > cubes_max[1]){
            is_possible = false;
        }
        if(cubes > cubes_min_nr[1]) {
            cubes_min_nr[1] = cubes;
        }
    }

    if(substring.find("blue") != std::string::npos) {
        if(cubes > cubes_max[2]){
            is_possible = false;
        }
        if(cubes > cubes_min_nr[2]) {
            cubes_min_nr[2] = cubes;
        }
    }

    return is_possible;
}