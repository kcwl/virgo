#include "keyword.h"
#include "read_value.hpp"

namespace virgo
{
	void protocol::parse(std::fstream& ifs, std::size_t& column, std::size_t& row)
	{
		std::string keyword{};
		read_value<'{'>(ifs, keyword, column, row);

		name = keyword;

		seek<'{'>(ifs, column, row);

		int times = 2;

		while (times--)
		{
			read_value<'{'>(ifs, keyword, column, row, token::key);

			seek<'{'>(ifs, column, row);

			if (keyword == "request")
				request.parse(ifs, column, row);
			else if (keyword == "response")
				response.parse(ifs, column, row);
			else
				throw std::runtime_error("syntax error! missing request or response keyword!");
		}

		seek<'}'>(ifs, column, row);

		seek<';'>(ifs, column, row);
	}

	void protocol::generate(const std::string& file_name)
	{
		auto header_file = std::format("{}.h", file_name);
		auto src_file = std::format("{}.cpp", file_name);

		std::fstream ofs(header_file, std::ios::out);

		if (!ofs.is_open())
			return;

		ofs << "struct " << name << "_protocol\n";
		ofs << "{\n";
		ofs << "private:\n";
		ofs << "    struct " << request.name() << ";\n";
		ofs << "    struct " << response.name() << ";\n";
		ofs << "public:\n";
		ofs << "    using request = virgo::" << request.type() << "::request<" << request.name() << ">;\n";
		ofs << "    using response = virgo::" << response.type() << "::request<" << request.name() << ">;\n";
		ofs << "};\n";

		std::fstream src_ofs(src_file, std::ios::out);
		if (!src_ofs.is_open())
			return;

		request.generate(name, src_ofs);

		response.generate(name, src_ofs);
	}

	void structure::parse(std::fstream& ifs, std::size_t column, std::size_t row)
	{
		read_value<'{'>(ifs, name_, column, row);

		seek<'{'>(ifs, column, row);

		while (!ifs.eof())
		{
			try
			{
				seek<'}'>(ifs, column, row);
				seek<';'>(ifs, column, row);
				return;
			}
			catch (...)
			{
				scopes_.push_back({});
				auto& content = scopes_.back();
				read_value<' '>(ifs, content.first, column, row);
				seek<' '>(ifs, column, row);
				read_value<';'>(ifs, content.second, column, row);
				seek<';'>(ifs, column, row);
			}
		}
	}

	void structure::generate(std::fstream& ofs)
	{
		ofs << "struct " << name_ << "\n";
		ofs << "{\n";
		for (auto& [type, name] : scopes_)
		{
			ofs << "    " << type << " " << name << ";\n";
		}
		ofs << "};\n";
	}

	void enum_struct::parse(std::fstream& ifs, std::size_t column, std::size_t row)
	{
		read_value<'{'>(ifs, name_, column, row);

		seek<'{'>(ifs, column, row);

		while (!ifs.eof())
		{
			try
			{
				seek<'}'>(ifs, column, row);
				seek<';'>(ifs, column, row);
				return;
			}
			catch (...)
			{
				scopes_.push_back({});
				auto& content = scopes_.back();
				read_value<';'>(ifs, content, column, row);
				seek<';'>(ifs, column, row);
			}
		}
	}

	void enum_struct::generate(std::fstream& ofs)
	{
		ofs << "enum class " << name_ << "\n";
		ofs << "{\n";
		for (auto& name : scopes_)
		{
			ofs << "    " << name << ",\n";
		}
		ofs << "};\n";
	}

	bool check_keyword_type(const std::string& name)
	{
		return true;
	}
} // namespace virgo