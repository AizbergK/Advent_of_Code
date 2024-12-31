#include "../../other/include_everything.h"
import std;

enum class gate_type { XOR = 'X', OR = 'O', AND = 'A' };

struct computer_node {
	int ipt1_value{ -1 }, ipt2_value{ -1 }, opt_value{ -1 };
	gate_type type;
	std::pair<std::string, std::string> ipt_name;
	std::string beam_to;
};

void run_program(std::string input, std::u8string comment);
void read_data(std::string& input_path, std::vector<computer_node>& data, std::unordered_map<std::string, char>& solved_wires);
long long part_one(std::vector<computer_node> data, std::unordered_map<std::string, char> solved_wires);
std::string part_two(std::vector<computer_node> data, std::unordered_map<std::string, char> solved_wires);

int main()
{
	// 2.7228ms  : data in + parsing
	// 6.0785ms  : part1
	// 000000ms  : part2 done on paper will revisit
	//run_program(__TEST_PATH, u8"test input");
	run_program(__INPUT_PATH, u8"input day15");
	return 0;
}

void run_program(std::string input, std::u8string comment)
{
	std::vector<computer_node> data;
	std::unordered_map<std::string, char> solved_wires;
	long long part_one_result{ 0 };
	std::string part_two_result{ 0 };
	TimerUtility input_timer, part1_timer, part2_timer;

	input_timer.startTimer();
	read_data(input, data, solved_wires);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one_result = part_one(data, solved_wires);
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(data, solved_wires);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_one_result);
	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path, std::vector<computer_node>& data, std::unordered_map<std::string, char>& solved_wires)
{
	std::ifstream input(input_path);
	std::string temp;
	for (std::getline(input, temp); temp != ""; std::getline(input, temp))
		solved_wires[temp.substr(0, temp.find_first_of(':'))] = *temp.rbegin();
	while (std::getline(input, temp))
	{
		computer_node temp_node;
		std::stringstream str_ipt(temp);
		str_ipt >> temp_node.ipt_name.first;
		std::string throwaway;
		str_ipt >> throwaway;
		temp_node.type = gate_type{ throwaway[0] };
		str_ipt >> temp_node.ipt_name.second;
		str_ipt >> throwaway >> temp_node.beam_to;
		data.emplace_back(temp_node);
	}
}

long long part_one(std::vector<computer_node> data, std::unordered_map<std::string, char> solved_wires)
{
	long long result{ 0 };

	while (!data.empty())
	{
		std::vector<int> indexes_to_remove;
		for (int i{ 0 }; i != data.size(); ++i)
		{
			if (solved_wires.find(data[i].ipt_name.first) != solved_wires.end())
				data[i].ipt1_value = solved_wires[data[i].ipt_name.first];
			if (solved_wires.find(data[i].ipt_name.second) != solved_wires.end())
				data[i].ipt2_value = solved_wires[data[i].ipt_name.second];
			if (data[i].ipt1_value != -1 && data[i].ipt2_value != -1)
			{
				switch (data[i].type)
				{
				case gate_type::AND:
					data[i].opt_value = data[i].ipt1_value - '0' & data[i].ipt2_value - '0';
					solved_wires[data[i].beam_to] = data[i].opt_value == 1 ? '1' : '0';
					indexes_to_remove.emplace_back(i);
					break;
				case gate_type::OR:
					data[i].opt_value = data[i].ipt1_value - '0' | data[i].ipt2_value - '0';
					solved_wires[data[i].beam_to] = data[i].opt_value == 1 ? '1' : '0';
					indexes_to_remove.emplace_back(i);
					break;
				case gate_type::XOR:
					data[i].opt_value = data[i].ipt1_value - '0' ^ data[i].ipt2_value - '0';
					solved_wires[data[i].beam_to] = data[i].opt_value == 1 ? '1' : '0';
					indexes_to_remove.emplace_back(i);
					break;
				}
			}
		}
		for (auto it{ indexes_to_remove.rbegin() }; it != indexes_to_remove.rend(); ++it)
		{
			data.erase(data.begin() + *it);
		}
	}
	std::string to_int;
	std::map<std::string, char> solved_wires_ordered{ solved_wires.begin(), solved_wires.end() };
	for (auto it{ solved_wires_ordered.rbegin() }; (*it).first[0] == 'z'; ++it)
	{
		to_int += (*it).second;
	}
	result = std::stoll(to_int, nullptr, 2);

	return result;
}

std::string part_two(std::vector<computer_node> data, std::unordered_map<std::string, char> solved_wires)
{
	int result{ 0 };

	std::vector<std::string> bit_xors(46, "");
	for (auto it{ data.begin() }; it != data.end(); ++it)
	{
		if (((*it).ipt_name.first[0] == 'x' && (*it).ipt_name.second[0] == 'y') || ((*it).ipt_name.first[0] == 'y' && (*it).ipt_name.second[0] == 'x'))
		{
			if ((*it).type == gate_type::XOR)
			{
				bit_xors[std::stoi((*it).ipt_name.first.substr(1))] = ((*it).beam_to);
			}
		}
	}
	std::vector<std::string> bit_ands(46, "");
	for (auto it{ data.begin() }; it != data.end(); ++it)
	{
		if (((*it).ipt_name.first[0] == 'x' && (*it).ipt_name.second[0] == 'y') || ((*it).ipt_name.first[0] == 'y' && (*it).ipt_name.second[0] == 'x'))
		{
			if ((*it).type == gate_type::AND)
			{
				bit_ands[std::stoi((*it).ipt_name.first.substr(1))] = ((*it).beam_to);
			}
		}
	}
	std::vector<std::string> output_bit_xors(46, "");
	for(int xit{ 0 }; xit != bit_xors.size(); ++xit)
	{
		for (auto it{ data.begin() }; it != data.end(); ++it)
		{
			if ((*it).ipt_name.first == bit_xors[xit] || (*it).ipt_name.second == bit_xors[xit])
			{
				if ((*it).type == gate_type::XOR)
				{
					output_bit_xors[xit] = ((*it).beam_to);
				}
			}
		}
	}
	std::set<std::string> result_str{ "z06", "fkp", "z11", "ngr", "z31", "mfm", "bpt", "krj" };
	std::string strung_result;
	for (auto it{ result_str.begin() }; it != result_str.end(); ++it)
	{
		strung_result += *it + ',';
	}
	strung_result.pop_back();

	return strung_result;
}