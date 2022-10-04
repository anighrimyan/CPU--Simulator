/**
 * @file entity.h
 * this file defines the class Entity
 */

#ifndef CPU_SIMUL_H
#define CPU_SIMUL_H

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace cpu_simul {
	class CPU_Simul {
	public:
		CPU_Simul();
		~CPU_Simul();

	public:
		void parsing();
		void call_operation(int keyword_index, std::string& reg_name, std::string& third_token);
		void call_jump_operation(int keyword_index, std::string& lable_token);
		void m_mov(std::string& reg_name, std::string& third_token);
		void m_add(std::string& reg_name, std::string& third_token);
		void m_sub(std::string& reg_name, std::string& third_token);
		void m_div(std::string& reg_name, std::string& third_token);
		void m_mul(std::string& reg_name, std::string& third_token);
		void m_and(std::string& reg_name, std::string& third_token);
		void m_or(std::string& reg_name, std::string& third_token);
		void m_xor(std::string& reg_name, std::string& third_token);
		void m_cmp(std::string& reg_name, std::string& third_token);
		void m_jump(std::string& lable_token);

	private:
		int is_keyword(const std::string& token);
		void push_lable(std::string& token);
		bool is_register(const std::string& new_token);
		bool is_number(const std::string& token);
		int from_string_to_int(std::string& token);
		bool is_lable(const std::string& token);
		void operators(std::string& token);
		void jump_operators(std::string& token);
		void lable_parsing(std::string& token);
		bool is_r9_set();
		bool is_r10_set();
		std::fstream fin;
		std::fstream fout;
		std::vector<std::string> m_keyword;
		std::vector<std::string> m_lable;
		std::map<std::string, int> m_reg;
		std::map<std::string, int>::iterator it;
	};
} // namespace cpu_simul
#endif // !CPU_SIMUL_H