#pragma once
#include <string>
#include <vector>

namespace aquarius
{
	struct statement
	{
		std::string type_str;

		std::string sub_type;

		std::string name_str;

		std::string sequence;

		std::vector<statement> seqs;
	};

	enum erro_code
	{
		success = 0,

		stop_character,

		right_brace_match,

		left_brace_match,

		eqyual_match,

		syntax_error
	};

	std::size_t md5(const std::string& str);
}