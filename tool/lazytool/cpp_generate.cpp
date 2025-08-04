#include "cpp_generate.h"

namespace aquarius
{
	cpp_generator::cpp_generator(const std::string& file_ename, const std::string& standard)
		: namespace_queue_()
		, file_name_(file_ename)
		, standard_(standard)
	{
		if (!create_file_stream())
		{
			throw std::runtime_error("create generate file stream failed!");
		}
	}

	cpp_generator::~cpp_generator()
	{
		generate_namespace_end();
	}

	void cpp_generator::generate(statement_base* state)
	{
		if (!state)
			return;

		auto type_str = state->type_str;

		if (type_str == "message")
		{
			generate_message(static_cast<message_statement*>(state));
		}
		else if (type_str == "enum")
		{
			generate_enum(static_cast<enum_statement*>(state));
		}
		else if (type_str == "tcp" || type_str == "udp" || type_str == "http")
		{
			generate_rpc(static_cast<rpc_statement*>(state));
		}
		else if (type_str == "import")
		{
			generate_single(state);
		}
		else if (type_str == "namespace")
		{
			generate_namespace_begin(state);
		}
	}

	void cpp_generator::generate_header()
	{
		ofs_ << "#pragma once\n";
		ofs_ << "#include <aquarius_protocol.hpp>\n\n";
	}

	bool cpp_generator::create_file_stream()
	{
		auto file_name = file_name_.substr(0, file_name_.find_last_of("."));
		if (file_name_.empty())
			return false;

		file_name += ".proto.hpp";

		ofs_.open(file_name, std::ios::out | std::ios::binary);
		if (!ofs_.is_open())
			return false;

		return true;
	}

	void cpp_generator::generate_namespace_begin(statement_base* state)
	{
		ofs_ << std::endl << "namespace " << state->type_str << " {";

		namespace_queue_.push('}');
	}

	void cpp_generator::generate_namespace_end()
	{
		while (!namespace_queue_.empty())
		{
			ofs_ << namespace_queue_.front();
			namespace_queue_.pop();
		}
	}

	void cpp_generator::generate_enum(enum_statement* state)
	{
		if (standard_ != "c++03" || standard_ != "c++98")
		{
			ofs_ << std::endl << "enum class " << state->name_str;
		}
		else
		{
			ofs_ << std::endl << "enum " << state->name_str;
		}

		ofs_ << std::endl << "{";

		for (auto& s : state->value)
		{
			if (s.empty())
				continue;

			ofs_ << std::endl << '\t' << s << ",";
		}

		ofs_.seekp(-1, std::ios::cur);

		ofs_ << std::endl << "};";
	}

	void cpp_generator::generate_message(message_statement* state)
	{
		ofs_ << std::endl << "struct " << state->name_str;
		ofs_ << std::endl << "{" << std::endl;
		for (auto& s : state->seqs)
		{
			if (s->type_str.empty())
				continue;

			if (s->sub_type == "repeated")
			{
				ofs_ << "\tstd::vector<" << convert_type(s->type_str) << ">";
			}
			else
			{
				ofs_ << '\t' << convert_type(s->type_str);
			}

			ofs_ << " " << s->name_str << ";\n";
		}

		ofs_ << "};";

		generate_template(state);
	}

	void cpp_generator::generate_template(message_statement* state)
	{
		//ofs_ << std::endl << "template <>";
		//ofs_ << std::endl << "struct aquarius::reflect<" << state->name_str << ">";
		//ofs_ << std::endl << "{";
		//ofs_ << std::endl << "\tusing value_type = " << state->name_str << ";";
		//ofs_ << std::endl << "\tconstexpr static std::string_view topic()";
		//ofs_ << std::endl << "\t{";
		//ofs_ << std::endl << "\t\treturn \"" << state->name_str << "\"sv;";
		//ofs_ << std::endl << "\t}";
		//ofs_ << std::endl;
		//ofs_ << std::endl << "\tconstexpr static std::array<std::string_view, " << state->seqs.size() << "> fields()";
		//ofs_ << std::endl << "\t{";
		//ofs_ << std::endl << "\t\treturn {";
		//for (auto& s : state->seqs)
		//{
		//	if (s->name_str.empty())
		//		continue;

		//	ofs_ << "\"" << s->name_str << "\"sv, ";
		//}

		//if (!state->seqs.empty())
		//{
		//	ofs_.seekp(-2, std::ios::cur);
		//}

		//ofs_ << "};";

		//ofs_ << std::endl << "\t}";

		//ofs_ << std::endl << "};\n";
	}

	void cpp_generator::generate_single(statement_base* state)
	{
		if (state->type_str.empty())
			return;

		ofs_ << std::endl << '\t' << convert_type(state->type_str) << " " << state->name_str << ";\n";
	}

	void cpp_generator::generate_rpc(rpc_statement* state)
	{
		std::string protocol("tcp");

		if (state->type_str == "udp")
		{
			protocol = "udp";
		}
		else if (state->type_str == "http")
		{
			protocol = "http";
		}

		ofs_ << "struct rpc_" << state->name_str << std::endl;
		ofs_ << "{\n";
		ofs_ << "\tconstexpr static auto id = " << state->number << ";\n";
		ofs_ << "\tusing request = aquarius::" << protocol << "_request<" << state->rpc.req << ">;\n";
		ofs_ << "\tusing response = aquarius::" << protocol << "_response<" << state->rpc.resp << ">;\n";
		ofs_ << "};\n";
		ofs_ << std::endl;
	}

	std::string cpp_generator::convert_type(const std::string& type_str)
	{
		std::string result = type_str;
		if (type_str == "int32")
		{
			result = "int32_t";
		}
		else if (type_str == "int64")
		{
			result = "int64_t";
		}
		else if (type_str == "uint32")
		{
			result = "uint32_t";
		}
		else if (type_str == "uint64")
		{
			result = "uint64_t";
		}
		else if (type_str == "string")
		{
			result = "std::string";
		}

		return result;
	}
} // namespace aquarius