#pragma once
#include "statement.hpp"
#include <fstream>
#include <functional>
#include <map>

namespace virgo
{
	class parse
	{
	public:
		parse();

	public:
		bool read_file(const std::string& file_name);

		std::vector<statement> get_statements();

	private:
		void regist_message_invoke(
			const std::string& type,
			const std::function<void(std::fstream&, statement&)>& invoke);

		void enum_invoke(std::fstream& ifs, statement& state);

		void message_invoke(std::fstream& ifs, statement& state);

		void rpc_invoke(std::fstream& ifs, statement& state);

		void common_invoke(std::fstream& ifs, statement& state);

		void repeated_invoke(std::fstream& ifs, statement& state);

		void parse_statement(std::fstream& ifs, statement* state_ptr);

		// void parse_message_statement(std::fstream& ifs, statement_base*& state_ptr);

		// void read_normal(std::fstream& ifs, statement_base* state_ptr);

		// void read_normal_bytes(std::fstream& ifs, statement_base* state_ptr);

		// void read_enum(std::fstream& ifs, enum_statement* state_ptr);

		// void read_message(std::fstream& ifs, message_statement* state_ptr);

		// void read_rpc(std::fstream& ifs, rpc_statement* state_ptr);

		// void read_repeated(std::fstream& ifs, statement_base* state_ptr);

		

		// void read_keyword(std::fstream& ifs, std::string& keyword);

		// void read_value(std::fstream& ifs, std::string& value);

		// void read_value_of(std::fstream& ifs, std::string& value);

		// void read_value_for(std::fstream& ifs, std::string& value);

		// void read_statement_keyword(std::fstream& ifs, std::string& value);

		// bool read_for_end(std::fstream& ifs);

		// bool check_type(std::string& type);

	private:
		std::vector<statement> statements_;

		std::map<std::string, std::function<void(std::fstream&, statement&)>> type_invokes_;

		std::size_t row_;

		std::size_t column_;
	};
} // namespace virgo
