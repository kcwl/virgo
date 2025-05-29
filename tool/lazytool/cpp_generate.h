#pragma once
#include "statement.hpp"
#include <fstream>
#include <queue>

namespace aquarius
{
	class cpp_generator
	{
	public:
		cpp_generator(const std::string& file_ename, const std::string& standard);

		~cpp_generator();

	public:
		void generate(statement_base* state);

		void generate_header();

	private:
		bool create_file_stream();

		void generate_namespace_begin(statement_base* state);

		void generate_namespace_end();

		void generate_enum(enum_statement* state);

		void generate_message(message_statement* state);

		void generate_template(message_statement* state);

		void generate_single(statement_base* state);

		void generate_rpc(rpc_statement* state);

	private:
		std::string convert_type(const std::string& type_str);

	private:
		std::queue<char> namespace_queue_;

		std::string file_name_;

		std::fstream ofs_;

		std::string standard_;

		std::queue<std::pair<std::string, std::string>> req_queue_;

		std::queue<std::pair<std::string, std::string>> resp_queue_;
	};
} // namespace aquarius