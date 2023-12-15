#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

int calibration_value(string , ofstream&);

int main() {

    ifstream input;
    ofstream output("./output_test.txt");
    input.open("./input.txt");

    int sum_calibration = 0;
    string calibration_string;

    while(!input.eof()) {
        input >> calibration_string;
        sum_calibration += calibration_value(calibration_string, output);
    }

    std::cout << sum_calibration;

    output.close();

    return 0;
}

int calibration_value(string calibration_string, ofstream& output) {
    int value = 0, min_pos = calibration_string.length(), max_pos = 0, min_pos_number = 0, max_pos_number = 0;
    const char *numbers[10] = 
    {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    int spelled_position[10];

    for (int i = 0; i < 10; i++) {
        spelled_position[i] = calibration_string.find(numbers[i]);
    }

    for (int i = 0; i < calibration_string.length(); i++)
    {
        if (calibration_string[i] >= 48 && calibration_string[i] <= 57)
        {
            if(spelled_position[(calibration_string[i] - 48)] > i || spelled_position[(calibration_string[i] - 48)] == -1){
                spelled_position[(calibration_string[i] - 48)] = i;
                min_pos_number = (calibration_string[i] - 48);
            }
            break;
        }
    }

    for (int i = 0; i < 10; i++) {
        if(spelled_position[i] != -1 && (spelled_position[i] < spelled_position[min_pos_number] || spelled_position[min_pos_number] == -1)) {
            min_pos_number = i;
        }
    }

    for (int i = 0; i < 10; i++) {
        spelled_position[i] = calibration_string.rfind(numbers[i]);
    }

    for (int i = calibration_string.length() - 1; i >= 0; i--) {
        if(calibration_string[i] >= 48 && calibration_string[i] <= 57 || spelled_position[(calibration_string[i] - 48)] == -1) {
            if(spelled_position[(calibration_string[i] - 48)] < i){
                spelled_position[(calibration_string[i] - 48)] = i;
                max_pos_number = (calibration_string[i] - 48);
            }
            break;
        }
    }

    for (int i = 0; i < 10; i++) {
        if(spelled_position[i] != -1 && (spelled_position[i] > spelled_position[max_pos_number] || spelled_position[max_pos_number] == -1)) {
            max_pos_number = i;
        }
    }

    value += min_pos_number * 10;
    value += max_pos_number;

    output << value << ' ' << calibration_string << endl;

    return value;
}