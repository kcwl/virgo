#pragma once
#include "statement.h"
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
		void generate(const statement& state);

	private:
		bool create_file_stream();

		void generate_namespace_begin(const statement& state);

		void generate_namespace_end();

		void generate_enum(const statement& state);

		void generate_message(const statement& state);

		void generate_template(const statement& state);

		void generate_single(const statement& state);

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