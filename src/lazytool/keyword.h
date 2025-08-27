#pragma once
#include "scope.h"
#include <string>

namespace virgo
{
	class keyword
	{
	public:
		virtual void generate(const std::string& /*file_name*/)
		{}
	};

	class protocol : public keyword
	{
	public:
		void parse(std::fstream& ifs, std::size_t& column, std::size_t& row);

		void generate(const std::string& file_name);

	public:
		std::string name;

		proto request;

		proto response;
	};

	class structure : public keyword
	{
	public:
		structure() = default;

	public:
		void parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs);

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

		void generate(std::fstream& ofs);

	private:
		std::string name_;

		std::vector<std::string> scopes_;
	};

	bool check_keyword_type(const std::string& name);
} // namespace virgo