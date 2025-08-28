#include "keyword.h"
#include "read_value.hpp"
#include <filesystem>

namespace virgo
{
	void protocol::parse(std::fstream& ifs, std::size_t& column, std::size_t& row)
	{
		std::string keyword{};
		read_value<'{'>(ifs, keyword, column, row);

		name_ = keyword;

		seek<'{'>(ifs, column, row);

		int times = 2;

		while (times--)
		{
			read_value<'{'>(ifs, keyword, column, row, token::key);

			seek<'{'>(ifs, column, row);

			if (keyword == "request")
				request_.parse(name_+"_req", ifs, column, row);
			else if (keyword == "response")
				response_.parse(name_ + "_resp", ifs, column, row);
			else
				throw std::runtime_error("syntax error! missing request or response keyword!");
		}

		seek<'}'>(ifs, column, row);

		seek<';'>(ifs, column, row);
	}

	void protocol::generate(std::fstream& ofs_h, std::fstream& ofs_s)
	{
		ofs_h << "struct " << name_ << "_protocol\n";
		ofs_h << "{\n";
		ofs_h << "private:\n";
		ofs_h << "    struct " << request_.name() << ";\n";
		ofs_h << "    struct " << response_.name() << ";\n";
		ofs_h << "public:\n";
		ofs_h << "    using request = virgo::" << request_.type() << "::request<" << request_.name() << ">;\n";
		ofs_h << "    using response = virgo::" << response_.type() << "::request<" << request_.name() << ">;\n";
		ofs_h << "};\n";

		request_.generate(name_, ofs_s);

		response_.generate(name_, ofs_s);
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

	void structure::generate(std::fstream& ofs_h, std::fstream& /*ofs_s*/)
	{
		ofs_h << "struct " << name_ << "\n";
		ofs_h << "{\n";
		for (auto& [type, name] : scopes_)
		{
			ofs_h << "    " << type << " " << name << ";\n";
		}
		ofs_h << "};\n";
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

	void enum_struct::generate(std::fstream& ofs_h, std::fstream& /*ofs_s*/)
	{
		ofs_h << "enum class " << name_ << "\n";
		ofs_h << "{\n";
		for (auto& name : scopes_)
		{
			ofs_h << "    " << name << ",\n";
		}
		ofs_h << "};\n";
	}

	bool check_keyword_type(const std::string& name)
	{
		return true;
	}
} // namespace virgo