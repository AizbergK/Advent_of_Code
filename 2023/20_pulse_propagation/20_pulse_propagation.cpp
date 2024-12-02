#include "../../other/include_everything.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

TimerUtility input_timer, part1_timer, part2_timer;
// 0.8477ms  : data in + parsing
// 24.652ms  : part1
// 343.56ms  : part2

// LOW SINGAL  = FALSE
// HIGH SIGNAL = TRUE
struct flip_flop
{
    bool state = false;
    std::vector<std::string> connections;
};

struct conjuction
{
    std::vector<std::string> connections;
    std::unordered_map<std::string, bool> inputs;
};

typedef std::unordered_map<std::string, flip_flop> flip_flops_map;
typedef std::unordered_map<std::string, conjuction> conjuctions_map;

void get_data(std::string &, std::pair<flip_flops_map, conjuctions_map> *, std::queue<std::pair<bool, std::string>> *);
void part1(long long *, std::pair<flip_flops_map, conjuctions_map>, std::queue<std::pair<bool, std::string>>);
void run_once(long long *, long long *, std::pair<flip_flops_map, conjuctions_map> *,
              std::queue<std::pair<bool, std::string>>);
void part2(long long *, std::pair<flip_flops_map, conjuctions_map>, std::queue<std::pair<bool, std::string>>);
bool run_once_part2(std::string, std::string, std::pair<flip_flops_map, conjuctions_map> *,
                    std::queue<std::pair<bool, std::string>>);

int main()
{
    std::string file_name = __INPUT_PATH;
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::pair<flip_flops_map, conjuctions_map> modules;
    std::queue<std::pair<bool, std::string>> sig_q;
    long long part1_total, part2_total;

    input_timer.startTimer();
    get_data(file_name, &modules, &sig_q);
    input_timer.getDuration("data in + parsing");

    part1_timer.startTimer();
    part1(&part1_total, modules, sig_q);
    part1_timer.getDuration("part1");

    part2_timer.startTimer();
    part2(&part2_total, modules, sig_q);
    part2_timer.getDuration("part2");

    std::cout << part1_total << '\n' << part2_total << '\n';

    return 0;
}

void get_data(std::string &file_name, std::pair<flip_flops_map, conjuctions_map> *modules,
              std::queue<std::pair<bool, std::string>> *sig_q)
{
    std::ifstream input;
    input.open(file_name);
    std::string temp_string, temp_key, temp_mod;
    std::unordered_map<std::string, char> temp_mods;
    flip_flops_map flip_flops;
    conjuctions_map conjuctions;

    while (std::getline(input, temp_string))
    {
        temp_mods.emplace(temp_string.substr(1, 2), temp_string[0]);
    }

    input.close();

    for (auto &&[key, value] : temp_mods)
    {
        switch (value)
        {
        case '%':
            flip_flops.emplace(value + key, flip_flop());
            break;
        case '&':
            conjuctions.emplace(value + key, conjuction());
            break;
        }
    }

    input.open(file_name);

    while (std::getline(input, temp_string))
    {
        temp_key = temp_string.substr(0, 3);
        temp_string = temp_string.substr(temp_string.find('>') + 2);
        temp_mod = temp_string.substr(0, temp_string.find(','));
        while (temp_string.find(',') != std::string::npos)
        {

            switch (temp_key[0])
            {
            case '%':
                flip_flops[temp_key].connections.push_back(temp_mods[temp_mod] + temp_mod);
                if (temp_mods[temp_mod] == '&')
                {
                    conjuctions[temp_mods[temp_mod] + temp_mod].inputs.emplace(temp_key, false);
                }
                break;
            case '&':
                conjuctions[temp_key].connections.push_back(temp_mods[temp_mod] + temp_mod);
                if (temp_mods[temp_mod] == '&')
                {
                    conjuctions[temp_mods[temp_mod] + temp_mod].inputs.emplace(temp_key, false);
                }
                break;
            case 'b':
                sig_q->emplace(false, temp_mods[temp_mod] + temp_mod);
                break;
            }
            temp_string = temp_string.substr(temp_string.find(',') + 2);
            temp_mod = temp_string.substr(0, temp_string.find(','));
        }
        switch (temp_key[0])
        {
        case '%':
            flip_flops[temp_key].connections.push_back(temp_mods[temp_mod] + temp_mod);
            if (temp_mods[temp_mod] == '&')
            {
                conjuctions[temp_mods[temp_mod] + temp_mod].inputs.emplace(temp_key, false);
            }
            break;
        case '&':
            conjuctions[temp_key].connections.push_back(temp_mods[temp_mod] + temp_mod);
            if (temp_mods[temp_mod] == '&')
            {
                conjuctions[temp_mods[temp_mod] + temp_mod].inputs.emplace(temp_key, false);
            }
            break;
        case 'b':
            sig_q->emplace(false, temp_mods[temp_mod] + temp_mod);
            break;
        }
    }
    modules->first = flip_flops;
    modules->second = conjuctions;
}

void part1(long long *total, std::pair<flip_flops_map, conjuctions_map> modules,
           std::queue<std::pair<bool, std::string>> sig_q)
{
    long long high_count = 0, low_count = 0, i = 1, remaining;

    run_once(&low_count, &high_count, &modules, sig_q);

    for (i; i < 1000; i++)
    {
        run_once(&low_count, &high_count, &modules, sig_q);
    }

    high_count *= 1000 / i;
    low_count *= 1000 / i;
    remaining = 1000 % i;

    for (i = 0; i < remaining; i++)
    {
        run_once(&low_count, &high_count, &modules, sig_q);
    }

    (*total) = low_count * high_count;
}

void run_once(long long *low_count, long long *high_count, std::pair<flip_flops_map, conjuctions_map> *modules,
              std::queue<std::pair<bool, std::string>> sig_q)
{
    (*low_count) += 1 + sig_q.size();
    bool signal, to_send, did_rx_recieve_low = false;
    std::string sent_to;
    while (!sig_q.empty())
    {
        signal = sig_q.front().first;
        sent_to = sig_q.front().second;
        switch (signal)
        {
        case false:
            switch (sent_to[0])
            {
            case '%':
                modules->first[sent_to].state = !modules->first[sent_to].state;
                for (auto &&conn : modules->first[sent_to].connections)
                {
                    sig_q.emplace(modules->first[sent_to].state, conn);
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = modules->first[sent_to].state;
                    }
                    modules->first[sent_to].state ? (*high_count)++ : (*low_count)++;
                }
                break;
            case '&':
                for (auto &&conn : modules->second[sent_to].connections)
                {
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = true;
                    }
                    sig_q.emplace(true, conn);
                    (*high_count)++;
                }
                break;
            }
            break;
        case true:
            switch (sent_to[0])
            {
            case '&':
                to_send = false;
                for (auto &&[key, value] : modules->second[sent_to].inputs)
                {
                    if (value == false)
                    {
                        to_send = true;
                        break;
                    }
                }

                for (auto &&conn : modules->second[sent_to].connections)
                {
                    sig_q.emplace(to_send, conn);
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = to_send;
                    }
                    to_send ? (*high_count)++ : (*low_count)++;
                }
                break;
            }
            break;
        }
        sig_q.pop();
    }
}

void part2(long long *total, std::pair<flip_flops_map, conjuctions_map> modules,
           std::queue<std::pair<bool, std::string>> sig_q)
{
    std::vector<int> temp_cycles;
    long long total_cycles = 1, i;
    *total = 1;
    bool some_condition = false;
    std::vector<std::string> to_check;
    std::string node_to_check_inputs;
    std::pair<flip_flops_map, conjuctions_map> temp_modules;

    for (auto &&[key, value] : modules.second)
    {
        for (auto &&outp : value.connections)
        {
            if (outp.substr(1, 2) == "rx")
            {
                node_to_check_inputs = key;
                for (auto &&ipt : value.inputs)
                {
                    to_check.push_back(ipt.first);
                }
            }
        }
    }

    for (auto &&ipt : to_check)
    {
        temp_modules = modules;
        i = 1;

        for (i; (!run_once_part2(node_to_check_inputs, ipt, &temp_modules, sig_q) && i < 1000'000); i++)
        {
            // std::cout << ipt;
        }
        temp_cycles.push_back(i);
    }

    for (auto &&cyc : temp_cycles)
    {
        total_cycles *= cyc;
    }
    *total = total_cycles;
}

bool run_once_part2(std::string node_to_check_inputs, std::string ipt,
                    std::pair<flip_flops_map, conjuctions_map> *modules, std::queue<std::pair<bool, std::string>> sig_q)
{
    bool signal, to_send, did_rx_recieve_low = false, condition;
    std::string sent_to;
    while (!sig_q.empty())
    {
        signal = sig_q.front().first;
        sent_to = sig_q.front().second;

        switch (signal)
        {
        case false:
            switch (sent_to[0])
            {
            case '%':
                modules->first[sent_to].state = !modules->first[sent_to].state;
                for (auto &&conn : modules->first[sent_to].connections)
                {
                    sig_q.emplace(modules->first[sent_to].state, conn);
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = modules->first[sent_to].state;
                    }
                }
                break;
            case '&':
                for (auto &&conn : modules->second[sent_to].connections)
                {
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = true;
                    }
                    sig_q.emplace(true, conn);
                }
                break;
            }
            break;
        case true:

            switch (sent_to[0])
            {
            case '&':
                to_send = false;
                for (auto &&[key, value] : modules->second[sent_to].inputs)
                {
                    if (value == false)
                    {
                        to_send = true;
                        break;
                    }
                }
                for (auto &&conn : modules->second[sent_to].connections)
                {
                    sig_q.emplace(to_send, conn);
                    if (conn[0] == '&')
                    {
                        modules->second[conn].inputs[sent_to] = to_send;
                    }
                }
                break;
            }
            break;
        }
        sig_q.pop();
        if (modules->second[node_to_check_inputs].inputs[ipt])
        {
            return true;
        }
    }

    return false;
}