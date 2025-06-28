#pragma once
#include "statement.hpp"
#include <fstream>
#include <queue>
#include <string>
#include <vector>

namespace aquarius
{
	class parse
	{
	public:
		parse();

	public:
		bool read_file(const std::string& file_name);

		std::vector<statement_base*> get_statements();

	private:
		void parse_statement(std::fstream& ifs, statement_base*& state_ptr);

		void parse_message_statement(std::fstream& ifs, statement_base*& state_ptr);

		void read_normal(std::fstream& ifs, statement_base* state_ptr);

		void read_normal_bytes(std::fstream& ifs, statement_base* state_ptr);

		void read_enum(std::fstream& ifs, enum_statement* state_ptr);

		void read_message(std::fstream& ifs, message_statement* state_ptr);

		void read_rpc(std::fstream& ifs, rpc_statement* state_ptr);

		void read_repeated(std::fstream& ifs, statement_base* state_ptr);

		std::string log(const std::string& str);

		void read_keyword(std::fstream& ifs, std::string& keyword);

		void read_value(std::fstream& ifs, std::string& value);

		void read_value_of(std::fstream& ifs, std::string& value);

		void read_value_for(std::fstream& ifs, std::string& value);

		void read_statement_keyword(std::fstream& ifs, std::string& value);

		bool read_for_end(std::fstream& ifs);

		bool check_type(std::string& type);

	private:
		std::vector<statement_base*> statements_;

		int row_;

		int column_;
	};
} // namespace aquarius
