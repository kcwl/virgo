#include "parse.h"
#include "read_value.hpp"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

namespace virgo
{
	parse::parse()
		: pros_()
		, regist_()
		, row_(0)
		, column_(0)
	{
		regist_.regist("protocol",
					   [this](std::fstream& ifs)
					   {
						   auto proto_ptr = std::make_shared<protocol>();
						   proto_ptr->parse(ifs, column_, row_);
						   pros_.push_back(proto_ptr);
					   });

		regist_.regist("struct",
					   [&](std::fstream& ifs)
					   {
						   auto struct_ptr = std::make_shared<structure>();
						   struct_ptr->parse(ifs, column_, row_);
						   pros_.push_back(struct_ptr);
					   });

		regist_.regist("enum",
					   [&](std::fstream& ifs)
					   {
						   auto enum_ptr = std::make_shared<enum_struct>();
						   enum_ptr->parse(ifs, column_, row_);
						   pros_.push_back(enum_ptr);
					   });
	}

	bool parse::read_file(const std::string& file_name, const std::string& output)
	{
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
				break;

			seek<' '>(ifs, column_, row_);

			auto f = regist_.invoke(keyword, column_, row_);

			try
			{
				f(ifs);
			}
			catch (std::exception& ep)
			{
				continue;
			}
		}

		std::filesystem::path out_path(file_name);

		std::filesystem::path final_output_path(output);
		final_output_path.append(out_path.filename().string());

		for (auto& p : pros_)
		{
			p->generate(final_output_path.string());
		}

		return true;
	}

} // namespace virgo