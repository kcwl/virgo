#pragma once
#include "scope.h"
#include <string>

namespace virgo
{
	class keyword
	{
	public:
		virtual void generate(std::fstream& /*ofs_h*/, std::fstream& /*ofs_s*/) {}
	};

	class protocol : public keyword
	{
	public:
		void parse(std::fstream& ifs, std::size_t& column, std::size_t& row);

		void generate(std::fstream& ofs_h, std::fstream& ofs_s);

	public:
		std::string name_;

		proto request_;

		proto response_;
	};

	class structure : public keyword
	{
	public:
		structure() = default;

	public:
		void parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs_h, std::fstream& ofs_s);

	private:
		std::string name_;

		std::vector<std::pair<std::string, std::string>> scopes_;
	};

	class enum_struct : public keyword
	{
	public:
		enum_struct() = default;

	public:
		void parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs_h, std::fstream& ofs_s);

	private:
		std::string name_;

		std::vector<std::string> scopes_;
	};

	bool check_keyword_type(const std::string& name);
} // namespace virgo