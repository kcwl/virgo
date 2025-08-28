#include "scope.h"
#include "read_value.hpp"
#include "log.h"
#include "keyword.h"

namespace virgo
{
	scope from_scope_string(const std::string& s)
	{
		if (s == "router")
		{
			return scope::s_router;
		}
		else if (s == "header")
		{
			return scope::s_header;
		}
		else if (s == "body")
		{
			return scope::s_body;
		}

		return scope::s_error;
	}

	bool router::parse(std::fstream& ifs, std::size_t column, std::size_t row)
	{
		if (ifs.eof())
			return false;

		read_value<' '>(ifs, key_, column, row, token::type);
		seek<' '>(ifs, column, row);

		read_path<';'>(ifs, value_, column, row);

		check_key(column, row);

		seek<';'>(ifs, column, row);

		return true;
	}

	void router::generate(std::fstream& ofs)
	{
		ofs << "constexpr static auto id = " << value_ << ";\n";
	}

	void router::check_key(std::size_t column, std::size_t row)
	{
		if (key_ == "tcp-key")
		{
			type_ = "tcp";
		}
		else if (key_ == "udp-key")
		{
			type_ = "udp";
		}
		else if (key_ == "http-regex")
		{
			type_ = "http";
		}
		else
		{
			throw std::runtime_error(log("check_value", std::format("not support router of {}!", type_), column, row));
		}
	}

	bool header::parse(std::fstream& ifs, std::size_t column, std::size_t row)
	{
		while (!ifs.eof())
		{
			try
			{
				seek<'}'>(ifs, column, row);
				seek<';'>(ifs, column, row);
				return true;
			}
			catch (...)
			{
				try
				{
					scopes_.push_back({});
					auto& content = scopes_.back();
					read_value<' '>(ifs, content.first, column, row, token::type);
					seek<' '>(ifs, column, row);

					if (!check_keyword_type(content.first))
						throw std::runtime_error(log("check_keyword_type",
													 std::format("not support keyword of {}!", content.first), column,
													 row));

					read_value<';'>(ifs, content.second, column, row);

					seek<';'>(ifs, column, row);
				}
				catch (...)
				{
					break;
				}
			}
		}
		return false;
	}

	void header::generate(std::fstream& ofs)
	{
		ofs << "struct header\n";
		ofs << "{\n";
		for (auto& [type, value] : scopes_)
		{
			if (type.empty())
				continue;

			ofs << "  " << type << " " << value << ";\n";
		}
		ofs << "};\n";
	}

	bool body::parse(std::fstream& ifs, std::size_t column, std::size_t row)
	{
		while (!ifs.eof())
		{
			try
			{
				seek<'}'>(ifs, column, row);
				seek<';'>(ifs, column, row);
				return true;
			}
			catch (...)
			{
				try
				{
					scopes_.push_back({});
					auto& content = scopes_.back();
					read_value<' '>(ifs, content.first, column, row, token::type);
					seek<' '>(ifs, column, row);

					read_value<';'>(ifs, content.second, column, row);

					seek<';'>(ifs, column, row);
				}
				catch (...)
				{
					break;
				}
			}
		}
		return false;
	}

	void body::generate(std::fstream& ofs)
	{
		ofs << "struct body\n";
		ofs << "{\n";
		for (auto& [type, value] : scopes_)
		{
			if (type.empty())
				continue;

			ofs << "  " << type << " " << value << ";\n";
		}
		ofs << "};\n";
	}

	bool proto::parse(const std::string& parent, std::fstream& ifs, std::size_t column, std::size_t row)
	{
		name_ = parent;

		bool result = false;

		while (!ifs.eof())
		{
			std::string keyword{};
			read_value<':'>(ifs, keyword, column, row, token::key);

			seek<':'>(ifs, column, row);

			auto tk = from_scope_string(keyword);

			switch (tk)
			{
				case scope::s_router:
					result = rt_.parse(ifs, column, row);
					try
					{
						seek<'}'>(ifs, column, row);
						seek<';'>(ifs, column, row);
					}
					catch (...)
					{
						result = false;
					}
					break;
				case scope::s_header:
					result = hr_.parse(ifs, column, row);
					break;
				case scope::s_body:
					result = by_.parse(ifs, column, row);
					break;
				default:
					break;
			}

			if (result)
				break;
		}

		return true;
	}

	void proto::generate(const std::string& name, std::fstream& ofs)
	{
		ofs << "struct " << name << "::" << name_ << "\n";
		ofs << "{\n";
		rt_.generate(ofs);
		hr_.generate(ofs);
		by_.generate(ofs);
		ofs << "};\n";
	}

	std::string proto::name() const
	{
		return name_;
	}

	std::string proto::type() const
	{
		return rt_.type_;
	}
} // namespace virgo