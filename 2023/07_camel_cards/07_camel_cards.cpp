#include "../../other/include_everything.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

TimerUtility program_timer;
// time to solve: 2.003ms

struct card
{
    std::string hand;
    int bid;
    int rank;
    int hash_value;
    long long abs_value;
};

int hand_rank(card);
int hash_value(std::string, std::vector<std::pair<char, int>>);
void set_default_hash_values(std::vector<std::pair<char, int>> *);
long long total_pot(std::vector<card>);
bool sort_func(card, card);
void joker(std::vector<std::pair<char, int>> *);

int main()
{

    program_timer.startTimer();

    std::vector<card> game;
    card individual_hand;
    std::vector<std::pair<char, int>> default_hash_value;
    set_default_hash_values(&default_hash_value);
    int hand_hash_max = pow(10, 7);

    std::ifstream input;
    input.open(__INPUT_PATH);

    while (input >> individual_hand.hand)
    {
        input >> individual_hand.bid;
        individual_hand.hash_value = hash_value(individual_hand.hand, default_hash_value);
        individual_hand.rank = hand_rank(individual_hand);
        individual_hand.abs_value = hand_hash_max * individual_hand.rank + individual_hand.hash_value;
        game.push_back(individual_hand);
    }

    std::sort(game.begin(), game.end(), sort_func);

    std::cout << total_pot(game) << std::endl;

    program_timer.getDuration();

    return 0;
}

int hand_rank(card individual_hand)
{

    int rank = 1;
    std::vector<std::pair<char, int>> hand_cards;
    std::pair<char, int> card_index;

    card_index.first = individual_hand.hand[0];
    card_index.second = 1;
    hand_cards.push_back(card_index);

    for (int i = 1; i < individual_hand.hand.length(); i++)
    {
        for (int j = 0; j <= hand_cards.size(); j++)
        {
            if (j == hand_cards.size())
            {
                card_index.first = individual_hand.hand[i];
                card_index.second = 1;
                hand_cards.push_back(card_index);
                break;
            }
            if (hand_cards[j].first == individual_hand.hand[i])
            {
                hand_cards[j].second++;
                break;
            }
        }
    }

    joker(&hand_cards);

    for (int i = 0; i < hand_cards.size(); i++)
    {
        if (hand_cards[i].second > 0)
        {
            rank += pow(hand_cards[i].second - 1, 2);
        }
    }

    return rank;
}

void joker(std::vector<std::pair<char, int>> *hand_cards)
{

    int joker_rank = 0, max_pos = 0, max = 0;
    for (int i = 0; i < (*hand_cards).size(); i++)
    {
        if ((*hand_cards)[i].first == 'J')
        {
            joker_rank = (*hand_cards)[i].second;
            (*hand_cards)[i].second = 0;
            break;
        }
    }
    for (int i = 0; i < (*hand_cards).size(); i++)
    {
        if ((*hand_cards)[i].second > max)
        {
            max = (*hand_cards)[i].second;
            max_pos = i;
        }
    }
    (*hand_cards)[max_pos].second += joker_rank;

    return;
}

void set_default_hash_values(std::vector<std::pair<char, int>> *default_hash_values)
{

    std::pair<char, int> card_value;

    for (int i = 1; i <= 13; i++)
    {
        switch (i)
        {
        case 1:
            card_value.first = '2';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 2:
            card_value.first = '3';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 3:
            card_value.first = '4';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 4:
            card_value.first = '5';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 5:
            card_value.first = '6';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 6:
            card_value.first = '7';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 7:
            card_value.first = '8';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 8:
            card_value.first = '9';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 9:
            card_value.first = 'T';
            card_value.second = i + 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 10:
            card_value.first = 'J';
            card_value.second = 1;
            (*default_hash_values).push_back(card_value);
            break;
        case 11:
            card_value.first = 'Q';
            card_value.second = i;
            (*default_hash_values).push_back(card_value);
            break;
        case 12:
            card_value.first = 'K';
            card_value.second = i;
            (*default_hash_values).push_back(card_value);
            break;
        case 13:
            card_value.first = 'A';
            card_value.second = i;
            (*default_hash_values).push_back(card_value);
            break;
        default:
            break;
        }
    }
}

int hash_value(std::string hand, std::vector<std::pair<char, int>> default_hash_values)
{

    int value = 0;
    int char_value;

    for (int i = 0; i < hand.length(); i++)
    {
        for (int j = 0; j < default_hash_values.size(); j++)
        {
            if (default_hash_values[j].first == hand[i])
            {
                char_value = default_hash_values[j].second;
            }
        }
        value += char_value * pow(default_hash_values.size(), hand.length() - (i + 1));
    }

    return value;
}

long long total_pot(std::vector<card> game)
{

    long long total = 0, bid, rank;

    for (int i = 0; i < game.size(); i++)
    {
        bid = game[i].bid;
        rank = i + 1;
        total += bid * rank;
    }

    return total;
}

bool sort_func(card obj1, card obj2)
{
    if (obj1.abs_value <= obj2.abs_value)
    {
        return true;
    }
    else
    {
        return false;
    }
}