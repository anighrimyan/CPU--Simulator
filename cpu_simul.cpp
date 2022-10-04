/**
 * @file cpu_simul.cpp
 * this file defines the class CPU_Simul
 */

#include "cpu_simul.h"
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

cpu_simul::CPU_Simul::CPU_Simul() {
	m_keyword = { "mov", "add", "sub", "div", "mul", "and", "or", "xor", "cmp", "jmp", "je", "jle", "jge", "jg", "jl", "end" };
	m_reg = { {"r1", 0}, {"r2", 0}, {"r3", 0}, {"r4", 0}, {"r5", 0}, {"r6", 0}, {"r7", 0}, {"r8", 0}, {"r9_flag", 0}, {"r10_flag", 0}};
	std::string path = "assembly_code";
	fin.open(path + ".txt");
	if (fin.is_open()) {
		std::cout << "File is open." << std::endl;
	}
	else {
		std::cout << "Error!!!" << std::endl;
	}
}

cpu_simul::CPU_Simul::~CPU_Simul() {
	fin.close();
	fout.close();
}

// This function parses the given string
void cpu_simul::CPU_Simul::parsing() {
	std::string token;
	int number{}, keyword_index{};
	while (!fin.eof()) {
		fin >> token;
		// Operations keyword.
		if (is_keyword(token) >= 0 && is_keyword(token) <= 8) {
			operators(token);
			token.clear();
		}
		// Jump command.
		else if (is_keyword(token) >= 9 && is_keyword(token) < m_keyword.size()) {
			jump_operators(token);
			token.clear();
		}
		// Lable push in vector.
		else if (is_keyword(token) == -1) {
			lable_parsing(token);
		}
		else {
			throw std::invalid_argument("ERROR: You have used an invalid keyword or syntax!");
		}
	}
}

// This function checks r9_flag value is 1 or 0.
bool cpu_simul::CPU_Simul::is_r9_set() {
	if (m_reg["r9_flag"] == 1) {
		return true;
	}
	return false;
}

// This function checks r10_flag value is 1 or 0.
bool cpu_simul::CPU_Simul::is_r10_set() {
	if (m_reg["r10_flag"] == 1) {
		return true;
	}
	return false;
}

// Parsing lable.
void cpu_simul::CPU_Simul::lable_parsing(std::string& token) {
	for (int i = 0; i < token.size() && token[token.size() - 1] == ':'; ++i) {
		if ((token[i] == ' ' || token[i] == '-' || token[i] == '+' || token[i] == '*' || token[i] == '/' || token[i] == '%')) {
			throw std::invalid_argument("ERROR: You have used an invalid syntax!");
		}
		else {
			push_lable(token);
			token.clear();
		}
	}
}

// Parsing jump.
void cpu_simul::CPU_Simul::jump_operators(std::string& token) {
	int keyword_index = is_keyword(token);
	std::string jump_token = token;
	std::string lable_token{};
	token.clear();
	fin >> token;
	token += ':';
	lable_token = token;
	while (!fin.eof()) {
		if (is_lable(token)) {
			fin.close();
			fin.open("assembly_code.txt");
			token.clear();
			while (!fin.eof()) {
				fin >> token;
				if (token == lable_token) {
					call_jump_operation(keyword_index, lable_token);
					return;
				}
			}
		}
		else {
			token.clear();
			fin >> token;
			if (token == lable_token) {
				call_jump_operation(keyword_index, lable_token);
				return;
			}
		}
	}
}

// Parsing operators and operations.
void cpu_simul::CPU_Simul::operators(std::string& token) {
	int keyword_index = is_keyword(token);
	if (is_keyword(token) >= 0 && is_keyword(token) < 9) {
		token.clear();
		fin >> token;
		std::string new_token{};
		for (int i = 0; i < token.length(); ++i) {
			if (token[i] != ',') {
				new_token += token[i];
			}
		}
		std::string reg_name{};
		if (is_register(new_token)) {
			reg_name = new_token;
		}
		else {
			throw std::invalid_argument("ERROR: You have used an invalid regiter name!");
		}
		token.clear();
		fin >> token;
		std::string third_token = token;
		if (is_number(third_token)) {
			call_operation(keyword_index, reg_name, third_token);
		}
		else if (is_register(third_token)) {
			call_operation(keyword_index, reg_name, third_token);
		}
		else {
			throw std::invalid_argument("ERROR: You have used an invalid keyword!");
		}
	}
}

// This function push lable name in m_lable vector.
void cpu_simul::CPU_Simul::push_lable(std::string& token) {
	m_lable.push_back(token);
}

// This function check the lable name in m_lable vector.
bool cpu_simul::CPU_Simul::is_lable(const std::string& token) {
	std::cout << "check_lable function" << std::endl;
	for (int i = 0; i < m_lable.size(); ++i) {
		if (m_lable[i] == token) {
			return true;
		}
	}
	return false;
}

// This function calls the corresponding jump action function.
void cpu_simul::CPU_Simul::call_jump_operation(int keyword_index, std::string& lable_token) {
	switch (keyword_index) {
	case 9: {
		m_jump(lable_token);
		break;
	}
	case 10: {
		while (is_r10_set()) {
			m_jump(lable_token);
		}
		break;
	}
	case 11: {
		while (is_r10_set() || is_r9_set()) {
			m_jump(lable_token);
		}
		break;
	}
	case 12: {
		while (is_r10_set() || is_r9_set()) {
			m_jump(lable_token);
		}
		break;
	}
	case 13: {
		while (is_r9_set()) {
			m_jump(lable_token);
		}
		break;
	}
	case 14: {
		while (is_r9_set()) {
			m_jump(lable_token);
		}
		break;
	}
	case 15:
	{
		std::cout << "Program end!!!" << std::endl;
		break;
	}
	}
}

// This function calls the corresponding action function.
void cpu_simul::CPU_Simul::call_operation(int keyword_index, std::string& reg_name, std::string& third_token) {
	switch (keyword_index) {
	case 0:
		m_mov(reg_name, third_token);
		break;
	case 1:
		m_add(reg_name, third_token);
		break;
	case 2:
		m_sub(reg_name, third_token);
		break;
	case 3:
		m_div(reg_name, third_token);
		break;
	case 4:
		m_mul(reg_name, third_token);
		break;
	case 5:
		m_and(reg_name, third_token);
		break;
	case 6:
		m_or(reg_name, third_token);
		break;
	case 7:
		m_xor(reg_name, third_token);
		break;
	case 8:
		m_cmp(reg_name, third_token);
		break;
	}
}

// This function calls unconditional jump.
void cpu_simul::CPU_Simul::m_jump(std::string& lable_token) {
	lable_token.clear();
	fin >> lable_token;
	parsing();
}

// In this function implemented two registers value comparison.
void cpu_simul::CPU_Simul::m_cmp(
	std::string& reg_name, std::string& third_token) {
	std::cout << "registers value comparison is " << std::endl;
	if (is_number(third_token)) {
		if (m_reg[reg_name] - from_string_to_int(third_token) > 0) {
			m_reg["r9_flag"] = 1;
		}
		else if (
			m_reg[reg_name] - from_string_to_int(third_token) == 0) {
			m_reg["10_flag"] = 1;
		}
	}
	else {
		if (m_reg[reg_name] - m_reg[third_token] > 0) {
			m_reg["r9_flag"] = 1;
		}
		else if (m_reg[reg_name] - m_reg[third_token] == 0) {
			m_reg["10_flag"] = 1;
		}
	}
	std::cout << "r9_flag " << m_reg["r9_flag"] << "\n"
		<< "r10_flag " << m_reg["r10_flag"] << std::endl;
}

// In this function implemented the bitwize XOR.
void cpu_simul::CPU_Simul::m_xor(
	std::string& reg_name, std::string& third_token) {
	std::cout << "bitwize XOR register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] ^= from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] ^= m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// In this function implemented the bitwize OR.
void cpu_simul::CPU_Simul::m_or(
	std::string& reg_name, std::string& third_token) {
	std::cout << "bitwize OR register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] |= from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] |= m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// In this function implemented the bitwize AND.
void cpu_simul::CPU_Simul::m_and(
	std::string& reg_name, std::string& third_token) {
	std::cout << "bitwize AND register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] &= from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] &= m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// This function subtract a value stored in a given register from a received value or a value in another register and stores the result in the same register.
void cpu_simul::CPU_Simul::m_mul(
	std::string& reg_name, std::string& third_token) {
	std::cout << " mul register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] *= from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] *= m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// This function division a value stored in a given register from a received value or a value in another register and stores the result in the same register.
void cpu_simul::CPU_Simul::m_div(
	std::string& reg_name, std::string& third_token) {
	std::cout << " div register value is " << std::endl;
	if (is_number(third_token) &&
		from_string_to_int(third_token) != 0) {
		m_reg[reg_name] /= from_string_to_int(third_token);
	}
	else if (is_register(third_token) && m_reg[third_token] != 0) {
		m_reg[reg_name] /= m_reg[third_token];
	}
	else {
		throw std::invalid_argument("ERROR: Can not divided to 0!");
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// This function subtract a value stored in a given register from a received value or a value in another register and stores the result in the same register.
void cpu_simul::CPU_Simul::m_sub(
	std::string& reg_name, std::string& third_token) {
	std::cout << " sub register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] -= from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] -= m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// This function adds a value stored in a given register to a received value or a value in another register and stores the result in the same register.
void cpu_simul::CPU_Simul::m_add(
	std::string& reg_name, std::string& third_token) {
	std::cout << " add register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] += from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] += m_reg[third_token];
	}
	std::cout << m_reg[reg_name] << std::endl;
}

// This function move a given value in a given register.
void cpu_simul::CPU_Simul::m_mov(
	std::string& reg_name, std::string& third_token) {
	std::cout << " mov register value is " << std::endl;
	if (is_number(third_token)) {
		m_reg[reg_name] = from_string_to_int(third_token);
	}
	else {
		m_reg[reg_name] = m_reg[third_token];
		std::cout << m_reg[reg_name] << std::endl;
	}
}

// This function convert from string to int.
int cpu_simul::CPU_Simul::from_string_to_int(std::string& token) {
	return (std::stoi(token));
}

// This function checks the received token is a number or not.
bool cpu_simul::CPU_Simul::is_number(const std::string& token) {
	for (int i = 0; i < token.length(); ++i) {
		if (token[i] < '0' || token[i] > '9') {
			return false;
		}
	}
	return true;
}

// This function checks the received token is register's name or not.
bool cpu_simul::CPU_Simul::is_register(const std::string& new_token) {
	for (auto& it : m_reg) {
		if (it.first == new_token) {
			return true;
		}
	}
	return false;
}

// This function checks the received token is keyword or not.
int cpu_simul::CPU_Simul::is_keyword(const std::string& token) {
	for (int i = 0; i < m_keyword.size(); ++i) {
		if (m_keyword[i] == token) {
			return i;
		}
	}
	return -1;
}
