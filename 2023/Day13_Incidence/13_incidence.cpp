#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include"../../timer_utility.h"

void get_input_data(std::ifstream *, std::vector<std::string> *, std::vector<std::string> *);
int verify_symmetry(std::vector<std::string> *);
bool symmetry_case(std::vector<std::string> *, int);
int verify_symmetry_smudge(std::vector<std::string> *);
bool symmetry_case_smudge(std::vector<std::string> *, int);
bool differs_by_one(std::string, std::string);

TimerUtility program_timer;
//time to solve: 0.9552ms

int main() {

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    int total_p1 = 0, total_p2 = 0;

    std::vector<std::string> pattern_vertical, pattern_horizontal;

    while (!input.eof()) {   
        get_input_data(&input, &pattern_vertical, &pattern_horizontal);
        total_p1 += 100 * verify_symmetry(&pattern_vertical);
        total_p1 += verify_symmetry(&pattern_horizontal);
        total_p2 += 100 * verify_symmetry_smudge(&pattern_vertical);
        total_p2 += verify_symmetry_smudge(&pattern_horizontal);
        pattern_horizontal.clear();
        pattern_vertical.clear();
    }

    std::cout << total_p1 << std::endl << total_p2 << std::endl;

    program_timer.getDuration();


    return 0;
}

void get_input_data(std::ifstream *input, std::vector<std::string> *pattern_vertical, std::vector<std::string> *pattern_horizontal) {

    std::string line, temp_char;

    std::getline((*input),line);
    for (int i = 0; i < line.length(); i++) {
        temp_char = line[i];
        pattern_horizontal->push_back(temp_char);
    }
    pattern_vertical->push_back(line);
    std::getline((*input),line);

    while (line != "" && !(*input).eof()) {
        pattern_vertical->push_back(line);
        for (int i = 0; i < line.length(); i++) {
            (*pattern_horizontal)[i] += line[i];
        } 
        std::getline((*input),line);
    } 
    if((*input).eof()) {
        pattern_vertical->push_back(line);
        for (int i = 0; i < line.length(); i++) {
            (*pattern_horizontal)[i] += line[i];
        } 
    }
    
    return;
}

int verify_symmetry(std::vector<std::string> *pattern) {

    int symmetry_line = 0;

    for (int i = 0; i < (*pattern).size(); i++) {
        if(symmetry_case(pattern, i)) {
            return i + 1;
        }
    }
    
    return symmetry_line;
}

bool symmetry_case(std::vector<std::string> *pattern, int start_index) {

    bool is_symmetry_line = false;

    for (int i = 0; ((start_index - i) >= 0) && ((start_index + i + 1) < (*pattern).size()) ; i++) {
        if((*pattern)[start_index - i].compare((*pattern)[start_index + i + 1])) {
            return false;
        }
        if((start_index - i == 0) || (start_index + i + 1 == (*pattern).size() - 1)) {
            is_symmetry_line = true;
        }
    }

    return is_symmetry_line;
}

int verify_symmetry_smudge(std::vector<std::string> *pattern) {

    int symmetry_line = 0;

    for (int i = 0; i < (*pattern).size(); i++) {
        if(symmetry_case_smudge(pattern, i)) {
            return i + 1;
        }
    }
    
    return symmetry_line;
}

bool symmetry_case_smudge(std::vector<std::string> *pattern, int start_index) {

    bool is_symmetry_line = false, wildcard = true;

    for (int i = 0; ((start_index - i) >= 0) && ((start_index + i + 1) < (*pattern).size()) ; i++) {
        if((*pattern)[start_index - i].compare((*pattern)[start_index + i + 1])) {
            if(differs_by_one((*pattern)[start_index - i], (*pattern)[start_index + i + 1]) && wildcard) {
                wildcard = false;
            } else {
            return false;
            }
        }
        if((start_index - i == 0) || (start_index + i + 1 == (*pattern).size() - 1)) {
            is_symmetry_line = true;
        }
    }
    if(wildcard) {
        is_symmetry_line = false;
    }

    return is_symmetry_line;
}

bool differs_by_one(std::string string_one, std::string string_two) {

    bool wildcard = true;

    for (int i = 0; i < string_one.length(); i++) {
        if(string_one[i] != string_two[i]) {
            if(wildcard) {
                wildcard = false;
            } else {
                return false;
            }
        }
    }
    
    return true;
}

