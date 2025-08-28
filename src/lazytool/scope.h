#pragma once
#include <fstream>
#include <vector>

namespace virgo
{
	enum class scope
	{
		s_router,
		s_header,
		s_body,
		s_error
	};

	scope from_scope_string(const std::string& s);

	class router
	{
	public:
		bool parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs);

	private:
		void check_key(std::size_t column, std::size_t row);

	public:
		std::string key_;

		std::string value_;

		std::string type_;
	};

	class header
	{
	public:
		bool parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs);

	public:
		std::vector<std::pair<std::string, std::string>> scopes_;
	};

	class body
	{
	public:
		bool parse(std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(std::fstream& ofs);

	public:
		std::vector<std::pair<std::string, std::string>> scopes_;
	};

	class proto
	{
	public:
		proto() = default;

	public:
		bool parse(const std::string& parent, std::fstream& ifs, std::size_t column, std::size_t row);

		void generate(const std::string& name, std::fstream& ofs);

		std::string name() const;

		std::string type() const;

	private:
		std::string name_;

		router rt_;

		header hr_;

		body by_;
	};
} // namespace virgo