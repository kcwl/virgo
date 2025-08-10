#pragma once
#include <format>
#include <fstream>
#include <string>

namespace virgo
{
	inline std::string log(const std::string& func_name, const std::string& str, std::size_t column, std::size_t row)
	{
		return std::format("[{}] {} at [column_:{},row_:{}]", func_name, str, column, row);
	}

	template <char... end>
	int read_value(std::fstream& ifs, std::string& value, std::size_t& column, std::size_t& row)
	{
		value.clear();

		int c{};

		while (!ifs.eof())
		{
			c = ifs.peek();

			if(c==-1)
				return 0;

			if (std::isalnum(c) || c == '_')
			{
				value += static_cast<char>(ifs.get());
				continue;
			}

			if (((c == end) || ...) && !value.empty())
			{
				return c;
			}

			if (c != '\t' && c != '\n' && c != '\r' && c != ' ')
			{
				break;
			}

			if (c == '\n')
			{
				column++;
				row = 0;
			}

			ifs.get();

			row++;
		}

		throw std::runtime_error(
			log("read_value", ifs.eof() ? "syntax error! file is eof!" : std::format("not support {} for value!", static_cast<char>(c)),
				column, row));
	}

	template<char... end>
	int read_path(std::fstream& ifs, std::string& value, std::size_t& column, std::size_t& row)
	{
		value.clear();

		int c{};

		while (!ifs.eof())
		{
			c = ifs.peek();

			if (((c == end) || ...) && !value.empty())
			{
				return c;
			}

			if (c != '\t' && c != '\n' && c != '\r' && c != ' ')
			{
				value += static_cast<char>(c);
			}

			if (c == '\n')
			{
				column++;
				row = 0;
			}

			ifs.get();

			row++;
		}

		return c;
	}

	template <char sp>
	void seek(std::fstream& ifs, std::size_t& column, std::size_t& row)
	{
		while (!ifs.eof())
		{
			auto c = ifs.peek();

			if (c == sp)
			{
				ifs.get();
				return;
			}

			if (c != ' ' && c != '\t' && c != '\n' && c != '\r')
			{
				break;
			}

			if (c == '\n')
			{
				column++;
				row = 0;
			}

			ifs.get();

			row++;
		}

		throw std::runtime_error(
			log("seek", ifs.eof() ? "file is eof!" : std::format("syntax error! missing {}!", sp), column, row));
	}
} // namespace virgo