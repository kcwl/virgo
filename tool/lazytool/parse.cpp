#include "parse.h"
#include <algorithm>
#include <format>
#include <iostream>

namespace aquarius
{
	parse::parse()
		: statements_()
		, row_(1)
		, column_(1)
	{}

	bool parse::read_file(const std::string& file_name)
	{
		std::fstream ifs(file_name, std::ios::in | std::ios::binary);
		if (!ifs.is_open())
		{
			std::cout << "file [" << file_name << "] not found\n";

			return false;
		}

		try
		{
			while (!ifs.eof())
			{
				statement_base* state_ptr = nullptr;

				parse_statement(ifs, state_ptr);

				statements_.push_back(state_ptr);
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;

			return false;
		}

		return true;
	}

	std::vector<statement_base*> parse::get_statements()
	{
		return statements_;
	}

	void parse::parse_statement(std::fstream& ifs, statement_base*& state_ptr)
	{
		std::string type_str{};

		read_statement_keyword(ifs, type_str);

		if (type_str.empty())
			return;

		if (type_str == "message")
		{
			state_ptr = new message_statement(type_str);

			read_message(ifs, static_cast<message_statement*>(state_ptr));
		}
		else if (type_str == "enum")
		{
			state_ptr = new enum_statement(type_str);

			read_enum(ifs, static_cast<enum_statement*>(state_ptr));
		}
		else if (type_str == "tcp" || type_str == "udp" || type_str == "http")
		{
			state_ptr = new rpc_statement(type_str);

			read_rpc(ifs, static_cast<rpc_statement*>(state_ptr));
		}
		else if (type_str == "import")
		{
			state_ptr = new statement_base(type_str);

			read_normal(ifs, state_ptr);
		}
		else if (type_str == "namespace")
		{
			state_ptr = new statement_base(type_str);

			read_normal(ifs, state_ptr);
		}
	}

	void parse::parse_message_statement(std::fstream& ifs, statement_base*& state_ptr)
	{
		std::string type_str{};

		read_keyword(ifs, type_str);

		if (state_ptr)
		{
			delete state_ptr;
			state_ptr = nullptr;
		}

		if (type_str == "bytes")
		{
			state_ptr = new statement_base("std::vector<uint8_t>");

			read_normal_bytes(ifs, state_ptr);
		}
		else if (type_str == "repeated")
		{
			state_ptr = new statement_base(type_str);

			read_repeated(ifs, state_ptr);
		}
		else
		{
			state_ptr = new statement_base(type_str);

			read_value(ifs, state_ptr->name_str);
		}
	}

	void parse::read_normal(std::fstream& ifs, statement_base* state_ptr)
	{
		read_value(ifs, state_ptr->name_str);
	}

	void parse::read_normal_bytes(std::fstream& ifs, statement_base* state_ptr)
	{
		read_value(ifs, state_ptr->name_str);

		statements_.push_back(state_ptr);
	}

	void parse::read_enum(std::fstream& ifs, enum_statement* state_ptr)
	{
		read_value_for(ifs, state_ptr->name_str);

		while (!ifs.eof())
		{
			if (read_for_end(ifs))
				break;

			state_ptr->value.push_back({});

			read_value(ifs, state_ptr->value.back());
		}
	}

	void parse::read_message(std::fstream& ifs, message_statement* state_ptr)
	{
		read_value_for(ifs, state_ptr->name_str);

		while (!ifs.eof())
		{
			if (read_for_end(ifs))
				break;

			statement_base* seq_state_ptr = nullptr;
			parse_message_statement(ifs, seq_state_ptr);

			state_ptr->seqs.push_back(seq_state_ptr);
		}
	}

	void parse::read_rpc(std::fstream& ifs, rpc_statement* state_ptr)
	{
		read_value_of(ifs, state_ptr->name_str);

		read_value_for(ifs, state_ptr->number);

		std::string key_word{};

		read_keyword(ifs, key_word);

		if (key_word == "rpc")
		{
			read_keyword(ifs, state_ptr->rpc.req);
			read_keyword(ifs, key_word);
			read_value(ifs, state_ptr->rpc.resp);
		}
		else
		{
			throw std::runtime_error(log("read_rpc maybe keword is not support!", __LINE__));
		}

		if (!read_for_end(ifs))
		{
			throw std::runtime_error(log("read_rpc maybe not end!",__LINE__));
		}
	}

	void parse::read_repeated(std::fstream& ifs, statement_base* state_ptr)
	{
		parse_message_statement(ifs, state_ptr);

		state_ptr->sub_type = "repeated";
	}

	std::string parse::log(const std::string& str, int line)
	{
		return std::format("column: {} row: {}\t {}\n", column_, row_, str);
	}

	void parse::read_keyword(std::fstream& ifs, std::string& keyword)
	{
		keyword.clear();

		while (!ifs.eof())
		{
			row_ += 1;

			auto c = ifs.peek();

			if (std::isalnum(c) || c == '_')
			{
				keyword += static_cast<char>(ifs.get());
			}
			else if (c == '\r' || c == '\n' || c == '\t' || c == -1)
			{
				ifs.get();
			}
			else if (c == ' ')
			{
				ifs.get();
				break;
			}
			else
			{
				throw std::runtime_error(log("read keyword syntax error!", __LINE__));
			}
		}
	}

	void parse::read_value(std::fstream& ifs, std::string& value)
	{
		value.clear();

		while (!ifs.eof())
		{
			row_ += 1;

			auto c = ifs.peek();

			if (std::isalnum(c) || c == '_')
			{
				value += static_cast<char>(ifs.get());
			}
			else if (c == ';')
			{
				ifs.get();
				break;
			}
			else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				ifs.get();

				if (c == '\n')
				{
					column_ += 1;
					row_ = 1;
				}
			}
			else
			{
				throw std::runtime_error(log("read_value syntax error!", __LINE__));
			}
		}
	}

	void parse::read_value_of(std::fstream& ifs, std::string& value)
	{
		value.clear();

		while (!ifs.eof())
		{
			row_ += 1;

			auto c = ifs.peek();

			if (std::isalnum(c) || c == '_')
			{
				value += static_cast<char>(ifs.get());
			}
			else if (c == '=')
			{
				ifs.get();

				break;
			}
			else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				ifs.get();
				if (c == '\n')
				{
					column_ += 1;
					row_ = 1;
				}
			}
			else if (c == '}')
			{
				ifs.get();
				break;
			}
			else
			{
				throw std::runtime_error(log("unexpected character", __LINE__));
			}
		}
	}

	void parse::read_value_for(std::fstream& ifs, std::string& value)
	{
		value.clear();

		if (ifs.eof())
		{
			throw std::runtime_error(log("read_value_for file is eof!", __LINE__));
		}

		while (!ifs.eof())
		{
			row_ += 1;
			auto c = ifs.peek();

			if (std::isalnum(c) || c == '_')
			{
				value += static_cast<char>(ifs.get());
			}
			else if (c == '{')
			{
				ifs.get();
				break;
			}
			else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				ifs.get();
				if (c == '\n')
				{
					column_ += 1;
					row_ = 1;
				}
			}
			else
			{
				throw std::runtime_error(log("read_value_for syntax error!", __LINE__));
			}
		}
	}

	void parse::read_statement_keyword(std::fstream& ifs, std::string& value)
	{
		read_keyword(ifs, value);

		if (value != "enum" && value == "message" && value == "rpc" && value != "import" && value != "namespace")
		{
			throw std::runtime_error(
				std::format("{} is not statement keyword! eg: enum message rpc import or namespace", value));
		}
	}

	bool parse::read_for_end(std::fstream& ifs)
	{
		if (ifs.eof())
			return false;

		while (!ifs.eof())
		{
			auto c = ifs.peek();
			if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				ifs.get();
				continue;
			}

			auto result = c == '}';

			if (result)
			{
				ifs.get();

				if (ifs.peek() == ';')
				{
					ifs.get();
				}
			}

			return c == '}';
		}

		return false;
	}

	bool parse::check_type(std::string& type)
	{
		auto iter = std::find_if(statements_.begin(), statements_.end(), [&](auto s) { return s->name_str == type; });

		return iter != statements_.end();
	}

} // namespace aquarius