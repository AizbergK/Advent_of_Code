#include "../../other/include_everything.h"
import std;

void run_program(std::string input, std::u8string comment, long long start_value);
void read_data(std::string& input_path);
void part_one();
long long part_two(long long start_value);

int main()
{
	// 119.30µs  : data in + parsing
	// 756.20µs  : part1
	// 9.5000µs  : part2
	run_program(__TEST_PATH, u8"test input", 1ll);
	run_program(__INPUT_PATH, u8"input day15", 35184372088831ll); //46 bits of 1
	return 0;
}

struct program_data {
	char ipt_buffer[20], opt_buffer[20] = "";
	long long registers[8] = { 0, 1, 2, 3, 0, 0 ,0 ,0 };
	int instr_ptr{ 0 }, ib_size{ 0 }, ob_size{ 0 };
}pdata;

inline void run_sub_program()
{
	goto jmptbl;

opcode0:
	++pdata.instr_ptr;
	pdata.registers[4] >>= pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'];
	++pdata.instr_ptr;
	goto jmptbl;

opcode1:
	++pdata.instr_ptr;
	pdata.registers[5] ^= pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'];
	++pdata.instr_ptr;
	goto jmptbl;

opcode2:
	++pdata.instr_ptr;
	pdata.registers[5] = pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'] % 8;
	++pdata.instr_ptr;
	goto jmptbl;

opcode3:
	++pdata.instr_ptr;
	pdata.registers[4] != 0 ? pdata.instr_ptr = pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'] : ++pdata.instr_ptr;
	if (pdata.instr_ptr > pdata.ib_size) return;
	goto jmptbl;

opcode4:
	++pdata.instr_ptr;
	pdata.registers[5] ^= pdata.registers[6];
	++pdata.instr_ptr;
	goto jmptbl;

opcode5:
	++pdata.instr_ptr;
	if (pdata.ob_size == 20) return;
	pdata.opt_buffer[pdata.ob_size] = (pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'] % 8) + '0';
	++pdata.ob_size;
	pdata.opt_buffer[pdata.ob_size] = '\0';
	++pdata.instr_ptr;
	goto jmptbl;

opcode6:
	++pdata.instr_ptr;
	pdata.registers[5] = pdata.registers[4] >> pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'];
	++pdata.instr_ptr;
	goto jmptbl;

opcode7:
	++pdata.instr_ptr;
	pdata.registers[6] = pdata.registers[4] >> pdata.registers[pdata.ipt_buffer[pdata.instr_ptr] - '0'];
	++pdata.instr_ptr;
	goto jmptbl;

jmptbl:
	switch (pdata.ipt_buffer[pdata.instr_ptr])
	{
	case '0':
		goto opcode0;
	case '1':
		goto opcode1;
	case '2':
		goto opcode2;
	case '3':
		goto opcode3;
	case '4':
		goto opcode4;
	case '5':
		goto opcode5;
	case '6':
		goto opcode6;
	case '7':
		goto opcode7;
	default:

		return;
	}
}

void run_program(std::string input, std::u8string comment, long long start_value)
{
	TimerUtility input_timer, part1_timer, part2_timer;
	long long part_two_result{ 0 };
	input_timer.startTimer();
	read_data(input);
	input_timer.getDuration(u8"input " + comment);

	part1_timer.startTimer();
	part_one();
	part1_timer.getDuration(u8"part1");

	part2_timer.startTimer();
	part_two_result = part_two(start_value);
	part2_timer.getDuration(u8"part2");

	std::println("{0}", part_two_result);
	std::println();
}

void read_data(std::string& input_path)
{
	pdata.registers[0] = 0;
	pdata.registers[1] = 1;
	pdata.registers[2] = 2;
	pdata.registers[3] = 3;
	pdata.registers[4] = 0;
	pdata.registers[5] = 0;
	pdata.registers[6] = 0;
	pdata.registers[7] = 0;
	pdata.ib_size = 0;
	pdata.ob_size = 0;
	pdata.instr_ptr = 0;

	std::ifstream input(input_path);
	std::string temp;
	std::getline(input, temp);
	pdata.registers[4] = std::stoi(temp.substr(temp.find_first_of(':') + 1));
	std::getline(input, temp);
	pdata.registers[5] = std::stoi(temp.substr(temp.find_first_of(':') + 1));
	std::getline(input, temp);
	pdata.registers[6] = std::stoi(temp.substr(temp.find_first_of(':') + 1));
	std::getline(input, temp);
	std::getline(input, temp);
	temp = temp.substr(temp.find_first_of(' ') + 1);
	int i{ 0 };
	for (i ; i < temp.size(); i += 2)
	{
		pdata.ipt_buffer[i / 2] = temp[i];
	}
	pdata.ib_size = i / 2;
}

void part_one()
{
	run_sub_program();
	for(int i{ 0 }; i != pdata.ob_size; ++i)
		std::cout << pdata.opt_buffer[i] << ',';
	std::cout << "\b \b\n";
}

bool check_partial()
{
	for (int i{ 0 }; i != pdata.ob_size; ++i)
	{
		if (pdata.opt_buffer[i] != pdata.ipt_buffer[16 - pdata.ob_size + i]) return false;
	}

	return true;
}

long long find_number(long long number, int index)
{
	for (int i{ 0 }; i != 8; ++i)
	{
		pdata.registers[4] = number + i;
		pdata.ob_size = 0;
		pdata.instr_ptr = 0;
		run_sub_program();
		if (!strcmp(pdata.opt_buffer, pdata.ipt_buffer))
		{
			return number + i;
		}
		if (check_partial())
		{
			long long temp_number = find_number((number + i) * 8, ++index);
			if (temp_number != -1)
				return temp_number;
		}

	}
	return -1;
}

long long part_two(long long start_value)
{
	long long result{ 0 };
	long long i{ start_value };
	if(i < INT_MAX)
	{
		for (i; strcmp(pdata.ipt_buffer, pdata.opt_buffer); ++i)
		{
			pdata.registers[4] = i;
			pdata.ob_size = 0;
			pdata.instr_ptr = 0;
			run_sub_program();
		}
		--i;
	}
	else
	{
		return find_number(0, 0);
	}

	return i;
}