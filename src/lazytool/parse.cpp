#include "parse.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include "read_value.hpp"

namespace virgo
{
	parse::parse()
		: pros_()
		, regist_()
		, row_(1)
		, column_(1)
	{
		regist_.regist("protocol", [this](std::fstream& ifs)
					  {
						  auto proto_ptr = std::make_shared<protocol>();
						  proto_ptr->parse(ifs, column_, row_);
						  pros_.push_back(proto_ptr);
					  });

		regist_.regist("struct", [&](std::fstream& ifs)
					  {
						  auto struct_ptr = std::make_shared<structure>();
						  struct_ptr->parse(ifs, column_, row_);
						  pros_.push_back(struct_ptr);
					  });

		regist_.regist("enum", [&](std::fstream& ifs)
					  {
						  auto enum_ptr = std::make_shared<enum_struct>();
						  enum_ptr->parse(ifs, column_, row_);
						  pros_.push_back(enum_ptr);
					  });
	}

	bool parse::read_file(const std::string& file_name)
	{
		if (!check_suffix(file_name, "virgo"))
		{
			std::cout << "file type error! please input *.virgo\n";

			return false;
		}

		std::fstream ifs(file_name, std::ios::in | std::ios::binary);

		if (!ifs.is_open())
		{
			std::cout << "file [" << file_name << "] not found\n";

			return false;
		}

		while (!ifs.eof())
		{
			std::string keyword{};
			if (read_value<' '>(ifs, keyword, column_, row_, token::key) == 0)
				return false;

			seek<' '>(ifs, column_, row_);

			regist_.invoke(keyword, column_, row_);
		}

		for (auto& p : pros_)
		{
			p->generate("protocol");
		}

		return true;
	}

	bool parse::check_suffix(const std::string& file_path, const std::string& suffix)
	{
		std::filesystem::path file(file_path);

		return file.extension().compare(suffix);
	}

} // namespace virgo