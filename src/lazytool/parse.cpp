#include "parse.h"
#include "stream.hpp"
#include <algorithm>
#include <format>
#include <iostream>

namespace virgo
{
	parse::parse()
		: statements_()
		, row_(1)
		, column_(1)
	{
		regist_message_invoke("message", [&] (std::fstream& ifs, statement& state) 
							  {
								  read_value<' ', '{'>(ifs, state.name, column_, row_);
								  seek<'{'>(ifs, column_, row_);

								  while (!ifs.eof())
								  {
									  try
									  {
										  seek<'}'>(ifs, column_, row_);
										  break;
									  }
									  catch (...)
									  {
										  state.states.push_back({});

										  read_value<' '>(ifs, state.states.back().type, column_, row_);

										  seek<' '>(ifs,column_, row_);

										  read_value<';'>(ifs,state.states.back().name, column_, row_);

										  seek<';'>(ifs, column_, row_);
									  }
								  }
							  });

		regist_message_invoke("enum", [&] (std::fstream& ifs, statement& state) 
							  {
								  read_value<' ', '{'>(ifs, state.name, column_, row_);

								  seek<'{'>(ifs, column_, row_);

								  while (!ifs.eof())
								  {
									  try
									  {
										  seek<'}'>(ifs, column_, row_);
										  break;
									  }
									  catch (...)
									  {
										  state.states.push_back({});

										  read_value<';'>(ifs, state.states.back().name, column_, row_);

										  seek<';'>(ifs,column_,row_);
									  }
								  }
							  });

		regist_message_invoke("tcp", [&] (std::fstream& ifs, statement& state)
							  {
								  read_value<' ', '='>(ifs, state.name, column_, row_);

								  seek<'='>(ifs, column_, row_);

								  std::string number{};

								  read_value<'{'>(ifs, number, column_, row_);

								  seek<'{'>(ifs, column_, row_);

								  state.number.emplace(number);

								  while (!ifs.eof())
								  {
									  try
									  {
										  seek<'}'>(ifs, column_, row_);
										  break;
									  }
									  catch (...)
									  {
										  state.states.push_back({});
										  auto& sub_state = state.states.back();

										  read_value<' '>(ifs, sub_state.type, column_, row_);

										  read_value<' '>(ifs, sub_state.name, column_, row_);

										  std::string return_key_word;

										  read_value<' '>(ifs, return_key_word, column_, row_);

										  if (return_key_word != "returns")
											  throw std::runtime_error(log("rpc", "syntax error! missing returns!", column_, row_));

										  std::string sub_type{};
										  read_value<' ', ';'>(ifs, sub_type, column_, row_);

										  state.sub_type.emplace(sub_type);

										  seek<';'>(ifs, column_, row_);
									  }
								  }
							  });

		regist_message_invoke("http", [&] (std::fstream& ifs, statement& state)
							  {
								  read_value<' ', '='>(ifs, state.name, column_, row_);

								  seek<'='>(ifs, column_, row_);

								  std::string number{};

								  read_path<'{'>(ifs, number, column_, row_);

								  seek<'{'>(ifs, column_, row_);

								  state.number.emplace(number);

								  while (!ifs.eof())
								  {
									  try
									  {
										  seek<'}'>(ifs, column_, row_);
										  break;
									  }
									  catch (...)
									  {
										  state.states.push_back({});
										  auto& sub_state = state.states.back();

										  read_value<' '>(ifs, sub_state.type, column_, row_);

										  read_value<' '>(ifs, sub_state.name, column_, row_);

										  std::string return_key_word;

										  read_value<' '>(ifs, return_key_word, column_, row_);

										  if (return_key_word != "returns")
											  throw std::runtime_error(log("rpc", "syntax error! missing returns!", column_, row_));

										  std::string sub_type{};
										  read_value<' ', ';'>(ifs, sub_type, column_, row_);

										  state.sub_type.emplace(sub_type);

										  seek<';'>(ifs, column_, row_);
									  }
								  }
							  });
	}

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
				statements_.push_back({});

				auto& state = statements_.back();

				parse_statement(ifs, &state);
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what() << std::endl;

			return false;
		}

		return true;
	}

	std::vector<statement> parse::get_statements()
	{
		return statements_;
	}

	void parse::regist_message_invoke(
		const std::string& type,
		const std::function<void(std::fstream&, statement&)>& invoke)
	{
		auto iter = type_invokes_.find(type);

		if (iter != type_invokes_.end())
			return;

		type_invokes_[type] = invoke;
	}

	void parse::common_invoke(std::fstream& ifs, statement& state)
	{
		read_value<';', ' '>(ifs, state.name, column_, row_);

		seek<';'>(ifs, column_, row_);
	}

	void parse::repeated_invoke(std::fstream& ifs, statement& state)
	{
		read_value<' '>(ifs, *state.sub_type, column_, row_);

		read_value<';', ' '>(ifs, state.name, column_, row_);

		seek<';'>(ifs, column_, row_);
	}

	void parse::parse_statement(std::fstream& ifs, statement* state_ptr)
	{
		std::string type_str{};

		read_value<' '>(ifs, type_str, column_, row_);

		if(type_str.empty())
			return;

		auto iter = type_invokes_.find(type_str);
		if (iter == type_invokes_.end())
		{
			throw std::runtime_error("unknow type [" + type_str + "]");
		}

		iter->second(ifs, *state_ptr);
	}

	// void parse::parse_message_statement(std::fstream& ifs, statement_base*& state_ptr)
	//{
	//	std::string type_str{};

	//	read_keyword(ifs, type_str);

	//	if (state_ptr)
	//	{
	//		delete state_ptr;
	//		state_ptr = nullptr;
	//	}

	//	if (type_str == "bytes")
	//	{
	//		state_ptr = new statement_base("std::vector<uint8_t>");

	//		read_normal_bytes(ifs, state_ptr);
	//	}
	//	else if (type_str == "repeated")
	//	{
	//		state_ptr = new statement_base(type_str);

	//		read_repeated(ifs, state_ptr);
	//	}
	//	else
	//	{
	//		state_ptr = new statement_base(type_str);

	//		read_value(ifs, state_ptr->name_str);
	//	}
	//}

	// void parse::read_normal(std::fstream& ifs, statement_base* state_ptr)
	//{
	//	read_value(ifs, state_ptr->name_str);
	// }

	// void parse::read_normal_bytes(std::fstream& ifs, statement_base* state_ptr)
	//{
	//	read_value(ifs, state_ptr->name_str);

	//	statements_.push_back(state_ptr);
	//}

	// void parse::read_enum(std::fstream& ifs, enum_statement* state_ptr)
	//{
	//	read_value_for(ifs, state_ptr->name_str);

	//	while (!ifs.eof())
	//	{
	//		if (read_for_end(ifs))
	//			break;

	//		state_ptr->value.push_back({});

	//		read_value(ifs, state_ptr->value.back());
	//	}
	//}

	// void parse::read_message(std::fstream& ifs, message_statement* state_ptr)
	//{
	//	read_value_for(ifs, state_ptr->name_str);

	//	while (!ifs.eof())
	//	{
	//		if (read_for_end(ifs))
	//			break;

	//		statement_base* seq_state_ptr = nullptr;
	//		parse_message_statement(ifs, seq_state_ptr);

	//		state_ptr->seqs.push_back(seq_state_ptr);
	//	}
	//}

	// void parse::read_rpc(std::fstream& ifs, rpc_statement* state_ptr)
	//{
	//	read_value_of(ifs, state_ptr->name_str);

	//	read_value_for(ifs, state_ptr->number);

	//	std::string key_word{};

	//	read_keyword(ifs, key_word);

	//	if (key_word == "rpc")
	//	{
	//		read_keyword(ifs, state_ptr->rpc.req);
	//		read_keyword(ifs, key_word);
	//		read_value(ifs, state_ptr->rpc.resp);
	//	}
	//	else
	//	{
	//		throw std::runtime_error(log("read_rpc maybe keword is not support!"));
	//	}

	//	if (!read_for_end(ifs))
	//	{
	//		throw std::runtime_error(log("read_rpc maybe not end!"));
	//	}
	//}

	// void parse::read_repeated(std::fstream& ifs, statement_base* state_ptr)
	//{
	//	parse_message_statement(ifs, state_ptr);

	//	state_ptr->sub_type = "repeated";
	//}

	// void parse::read_keyword(std::fstream& ifs, std::string& keyword)
	//{
	//	keyword.clear();

	//	while (!ifs.eof())
	//	{
	//		row_ += 1;

	//		auto c = ifs.peek();

	//		if (std::isalnum(c) || c == '_')
	//		{
	//			keyword += static_cast<char>(ifs.get());
	//		}
	//		else if (c == '\r' || c == '\n' || c == '\t' || c == -1)
	//		{
	//			ifs.get();
	//		}
	//		else if (c == ' ')
	//		{
	//			ifs.get();
	//			if(!keyword.empty())
	//				break;
	//		}
	//		else
	//		{
	//			throw std::runtime_error(log("read keyword syntax error!"));
	//		}
	//	}
	//}

	// void parse::read_value(std::fstream& ifs, std::string& value)
	//{
	//	value.clear();

	//	while (!ifs.eof())
	//	{
	//		row_ += 1;

	//		auto c = ifs.peek();

	//		if (std::isalnum(c) || c == '_')
	//		{
	//			value += static_cast<char>(ifs.get());
	//		}
	//		else if (c == ';')
	//		{
	//			ifs.get();
	//			break;
	//		}
	//		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	//		{
	//			ifs.get();

	//			if (c == '\n')
	//			{
	//				column_ += 1;
	//				row_ = 1;
	//			}
	//		}
	//		else
	//		{
	//			throw std::runtime_error(log("read_value syntax error!"));
	//		}
	//	}
	//}

	// void parse::read_value_of(std::fstream& ifs, std::string& value)
	//{
	//	value.clear();

	//	while (!ifs.eof())
	//	{
	//		row_ += 1;

	//		auto c = ifs.peek();

	//		if (std::isalnum(c) || c == '_')
	//		{
	//			value += static_cast<char>(ifs.get());
	//		}
	//		else if (c == '=')
	//		{
	//			ifs.get();

	//			break;
	//		}
	//		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	//		{
	//			ifs.get();
	//			if (c == '\n')
	//			{
	//				column_ += 1;
	//				row_ = 1;
	//			}
	//		}
	//		else if (c == '}')
	//		{
	//			ifs.get();
	//			break;
	//		}
	//		else
	//		{
	//			throw std::runtime_error(log("unexpected character"));
	//		}
	//	}
	//}

	// void parse::read_value_for(std::fstream& ifs, std::string& value)
	//{
	//	value.clear();

	//	if (ifs.eof())
	//	{
	//		throw std::runtime_error(log("read_value_for file is eof!"));
	//	}

	//	while (!ifs.eof())
	//	{
	//		row_ += 1;
	//		auto c = ifs.peek();

	//		if (std::isalnum(c) || c == '_')
	//		{
	//			value += static_cast<char>(ifs.get());
	//		}
	//		else if (c == '{')
	//		{
	//			ifs.get();
	//			break;
	//		}
	//		else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	//		{
	//			ifs.get();
	//			if (c == '\n')
	//			{
	//				column_ += 1;
	//				row_ = 1;
	//			}
	//		}
	//		else
	//		{
	//			throw std::runtime_error(log("read_value_for syntax error!"));
	//		}
	//	}
	//}

	// void parse::read_statement_keyword(std::fstream& ifs, std::string& value)
	//{
	//	read_keyword(ifs, value);

	//	if (value != "enum" && value == "message" && value == "rpc" && value != "import" && value != "namespace")
	//	{
	//		throw std::runtime_error(
	//			std::format("{} is not statement keyword! eg: enum message rpc import or namespace", value));
	//	}
	//}

	// bool parse::read_for_end(std::fstream& ifs)
	//{
	//	if (ifs.eof())
	//		return false;

	//	while (!ifs.eof())
	//	{
	//		auto c = ifs.peek();
	//		if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
	//		{
	//			ifs.get();
	//			continue;
	//		}

	//		auto result = c == '}';

	//		if (result)
	//		{
	//			ifs.get();

	//			if (ifs.peek() == ';')
	//			{
	//				ifs.get();
	//			}
	//		}

	//		return c == '}';
	//	}

	//	return false;
	//}

	// bool parse::check_type(std::string& type)
	//{
	//	auto iter = std::find_if(statements_.begin(), statements_.end(), [&](auto s) { return s->name_str == type; });

	//	return iter != statements_.end();
	//}

} // namespace virgo