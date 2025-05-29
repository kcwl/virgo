#pragma once
#include <string>
#include <vector>
#include <memory>

namespace aquarius
{
	struct statement_base
	{
		statement_base(const std::string& type)
			: type_str(type)
		{

		}

		std::string type_str;

		std::string name_str;

		std::string sub_type;
	};

	struct message_statement : statement_base
	{
		message_statement(const std::string& type)
			: statement_base(type)
		{

		}

		std::string sequence;

		std::vector<statement_base*> seqs;
	};

	struct enum_statement : statement_base
	{
		enum_statement(const std::string& type)
			: statement_base(type)
		{

		}

		std::vector<std::string> value;
	};

	struct rpc_statement : statement_base
	{
		rpc_statement(const std::string& type)
			: statement_base(type)
		{

		}

		struct impl_type
		{
			std::string req;
			std::string resp;
		};

		impl_type tcp;
		impl_type udp;
		impl_type http;
	};
}