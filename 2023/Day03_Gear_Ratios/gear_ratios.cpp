#include<iostream>
#include<fstream>
#include<string.h>
#include<string>
#include<math.h>

using namespace std;

int line_check(string, string, string);
bool number_check(string, int, int);
int line_check_gears(string, string ,string);
int gear_ratio(string, int, int*);

int main() {

    ifstream input;
    input.open("./input.txt");

    string first_line;
    string second_line;
    string third_line;
    
    int number, j, sum = 0, counter = 0, total_prod = 0, prod = 0;
    bool is_part_number = false;

    input >> second_line >> third_line;

    string dummy_string = string(second_line.length(), '.');

    sum += line_check(second_line, dummy_string, third_line);
    total_prod += line_check_gears(second_line, dummy_string, third_line);

    while (!input.eof())
    {
        first_line = second_line;
        second_line = third_line;
        input >> third_line;
        sum += line_check(second_line, first_line, third_line);
        total_prod += line_check_gears(second_line, first_line, third_line);
    }

    sum += line_check(third_line, second_line, dummy_string);
    total_prod += line_check_gears(third_line, second_line, dummy_string);

    cout << total_prod << endl;
    // cout << sum;

    return 0;
}

int line_check(string main_line, string top_line, string bottom_line) {

    int sum = 0, number = 0, j = 0;
    bool is_part_number = false;

    for (int i = 0; i < main_line.length(); i++) {

            if(main_line[i] >= 48 && main_line[i] <= 57) {

                if(i - 1 >= 0) {
                    if( main_line[i - 1] != 46) {
                        is_part_number = true;
                    }
                }

                j = i;
                while(main_line[j] >= 48 && main_line[j] <= 57 && j < main_line.length()) {
            
                    number *= 10;
                    number += (main_line[j] - 48);
                    j++;
                }
                if(j < main_line.length()) {
                    if( main_line[j] != 46) {
                        is_part_number = true;
                    }
                }
                if(!is_part_number) {
                is_part_number = (number_check(top_line, i, j) || number_check(bottom_line, i, j));
                }

                if(is_part_number) {
                    // cout << number << endl;
                    sum += number;
                }
                number = 0;
                i = j - 1;
                is_part_number = false;
            }
        }

    return sum;
}

bool number_check(string adjacent_line, int start, int stop) {

    if(start > 0 && adjacent_line[start - 1] != 46 && (adjacent_line[start - 1] < 48 || adjacent_line[start - 1] > 57 )) {
        return true;
    }

    for (int i = start; i < stop; i++) {
        if(adjacent_line[i] != 46 && (adjacent_line[i] < 48 || adjacent_line[i] > 57)) {
            return true;
        }
    }

    if(stop < adjacent_line.length() && adjacent_line[stop] != 46 && (adjacent_line[stop] < 48 || adjacent_line[stop] > 57 )) {
        return true;
    }

        return false;
}

int line_check_gears(string main_line, string top_line, string bottom_line) {

    int total_line_prod = 0, prod = 1, number = 0, counter = 0;
    bool is_part_number = false;

    for (int i = 0; i < main_line.length(); i++) {

            if(main_line[i] == 42) {
 
                prod *= gear_ratio(top_line, i, &counter);
                prod *= gear_ratio(bottom_line, i, &counter);

                if(i - 1 >= 0) {
                    if( main_line[i - 1] >= 48 && main_line[i - 1] <= 57) {
                        counter++;
                        for (int j = 0; i - j - 1 >= 0 && main_line[i - j - 1] >= 48 && main_line[i - j - 1] <= 57; j++) {
                            number += pow(10, j) * (main_line[i - j - 1] - 48);
                        }
                        prod *= number;
                        number = 0;
                    }
                }

                if(i + 1 < main_line.length()) {
                    if( main_line[i + 1] >= 48 && main_line[i + 1] <= 57) {
                        counter++;
                        for (int j = 1; i + j < main_line.length() && main_line[i + j] >= 48 && main_line[i + j] <= 57; j++) {
                            number *= 10;
                            number += (main_line[i + j] - 48);
                        }
                        prod *= number;
                        number = 0;
                    }
                }

                if(counter == 2) {
                    total_line_prod += prod;
                }
                counter = 0;
                prod = 1;
            }
        }
        

        return total_line_prod;
        
}

int gear_ratio(string adjacent_line, int i, int* counter) {

    int number1 = 0, number2 = 0;

    if(i - 1 >= 0) {
        if( adjacent_line[i - 1] >= 48 && adjacent_line[i - 1] <= 57) {
            (*counter)++;
            for (int j = 0; i - j - 1 >= 0 && adjacent_line[i - j - 1] >= 48 && adjacent_line[i - j - 1] <= 57; j++) {
                number1 += pow(10, j) * (adjacent_line[i - j - 1] - 48);
            }

            if(adjacent_line[i] >= 48 && adjacent_line[i] <= 57) {
                for (int j = 0; i + j < adjacent_line.length() && adjacent_line[i + j] >= 48 && adjacent_line[i + j] <= 57; j++) {
                    number1 *= 10;
                    number1 += (adjacent_line[i + j] - 48);
            }  
            return number1;
            } else if(adjacent_line[i + 1] >= 48 && adjacent_line[i + 1] <= 57) {
                (*counter)++;
                for (int j = 0; i + j + 1 < adjacent_line.length() && adjacent_line[i + j + 1] >= 48 && adjacent_line[i + j + 1] <= 57; j++) {
                    number2 *= 10;
                    number2 += (adjacent_line[i + j + 1] - 48);
                }
                return number1 * number2;
            } else {
                return number1;
            }
        } else {
            if(adjacent_line[i] >= 48 && adjacent_line[i] <= 57) {
                (*counter)++;
                for (int j = 0; i + j < adjacent_line.length() && adjacent_line[i + j] >= 48 && adjacent_line[i + j] <= 57; j++) {
                    number1 *= 10;
                    number1 += (adjacent_line[i + j] - 48);
                }  
                return number1;
            } else if(adjacent_line[i + 1] >= 48 && adjacent_line[i + 1] <= 57) {
                (*counter)++;
                for (int j = 0; i + j + 1 < adjacent_line.length() && adjacent_line[i + j + 1] >= 48 && adjacent_line[i + j + 1] <= 57; j++) {
                    number2 *= 10;
                    number2 += (adjacent_line[i + j + 1] - 48);
                }
                return number2;
            } else {
                return 1;
            }

        }
    } else {
        if(adjacent_line[i] >= 48 && adjacent_line[i] <= 57) {
            (*counter)++;
            for (int j = 0; i + j < adjacent_line.length() && adjacent_line[i + j] >= 48 && adjacent_line[i + j] <= 57; j++) {
                number1 *= 10;
                number1 += (adjacent_line[i + j] - 48);
            }  
        return number1;
        } else if(adjacent_line[i + 1] >= 48 && adjacent_line[i + 1] <= 57) {
            (*counter)++;
            for (int j = 0; i + j + 1 < adjacent_line.length() && adjacent_line[i + j + 1] >= 48 && adjacent_line[i + j + 1] <= 57; j++) {
                number2 *= 10;
                number2 += (adjacent_line[i + j + 1] - 48);
            }
            return number2;
        } else {
            return number1;
        }
    }

    return 1;

}