#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<math.h>
#include<vector>
#include"../../timer_utility.h"

TimerUtility program_timer;
//time to solve: 1.36ms

void extract_numbers(std::string, std::vector<int>*, std::vector<int>*);
int verify_card(std::vector<int>, std::vector<int>);
void count_cards(std::vector<int>*, int, int);
int add_total(std::vector<int> *, int);

int main() {

    program_timer.startTimer();

    std::ifstream input;
    input.open("./input.txt");
    std::string card;
    std::vector<int> lucky_numbers, card_numbers, cards_total(10000, 1);
    int total = 0, count, i = 0;

    while(!input.eof()) {
        getline(input, card);
        extract_numbers(card, &lucky_numbers, &card_numbers);
        count = verify_card(lucky_numbers, card_numbers);
        total += pow(2, count);
        count_cards(&cards_total, i, count + 1);
        lucky_numbers.clear();
        card_numbers.clear();
        i++;
    }

    std::cout << total << std::endl << add_total(&cards_total, i) << std::endl;

    program_timer.getDuration();

    return 0;
}

void extract_numbers(std::string card, std::vector<int> *lucky_numbers, std::vector<int> *card_numbers) {

    int next_number,
    start = card.find(":") + 2, 
    mid = card.find("|"), 
    end = card.length();

    while(start < mid) {
        next_number = stoi(card.substr(card.find_first_of("0123456789", start)));
        (*lucky_numbers).push_back(next_number);
        start += 3;
    }

    mid += 2;

    while(mid < end) {
        next_number = stoi(card.substr(card.find_first_of("0123456789", mid)));
        (*card_numbers).push_back(next_number);
        mid += 3;
    }

    return;
}

int verify_card(std::vector<int> lucky_numbers, std::vector<int> card_numbers) {

    int score = 0, power = -1;

    for (int i = 0; i < lucky_numbers.size(); i++) {
        for (int j = 0; j < card_numbers.size(); j++) {
            if(lucky_numbers[i] == card_numbers[j]) {
                power++;
            }
        }
    }
    return power;
}

void count_cards(std::vector<int>* total_cards, int current_pos, int count) {

    for (int i = current_pos + 1; i <= current_pos + count; i++) {
        (*total_cards)[i] += (*total_cards)[current_pos];
    }

    return;
}

int add_total(std::vector<int>* total_cards, int nr_of_cards) {

    int total = 0;

    for (int i = 0; i < nr_of_cards; i++) {
        total += (*total_cards)[i];
    }

    return total;
}