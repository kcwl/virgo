#pragma once
#include <algorithm>
#include <archive.hpp>
#include <cstdint>
#include <serialize.hpp>
#include <iostream>

namespace aquarius
{
	class basic_header
	{
	public:
		bool operator==(const basic_header& other) const
		{
			return total_package == other.total_package && pack_seq == other.pack_seq;
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			os << "header: " << total_package << "\t" << pack_seq << "\t";

			return os;
		}

	public:
		void to_binary(archive& ar)
		{
			binary::to(ar, total_package);
			binary::to(ar, pack_seq);
		}

		void from_binary(archive& ar)
		{
			total_package = binary::from<uint32_t>(ar);
			pack_seq = binary::from<uint32_t>(ar);
		}

		void swap(basic_header& other)
		{
			std::swap(total_package, other.total_package);
			std::swap(pack_seq, other.pack_seq);
		}

	public:
		uint32_t total_package;

		uint32_t pack_seq;
	};

	class basic_request_header : public basic_header
	{
		using base_type = basic_header;

	public:
		bool operator==(const basic_request_header& other) const
		{
			return base_type::operator==(other) && type == other.type;
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			os << type;

			return os;
		}

	public:
		void to_binary(archive& ar)
		{
			base_type::to_binary(ar);

			binary::to(ar, type);
		}

		void from_binary(archive& ar)
		{
			base_type::from_binary(ar);

			type = binary::from<uint32_t>(ar);
		}

		void swap(basic_request_header& other)
		{
			basic_header::swap(other);

			std::swap(type, other.type);
		}

	public:
		uint32_t type;
	};

	class basic_response_header : public basic_header
	{
		using base_type = basic_header;

	public:
		bool operator==(const basic_response_header& other) const
		{
			return base_type::operator==(other) && result == other.result;
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			os << result;

			return os;
		}

	public:
		void to_binary(archive& ar)
		{
			base_type::to_binary(ar);

			binary::to(ar, result);
		}

		void from_binary(archive& ar)
		{
			base_type::from_binary(ar);

			result = binary::from<uint32_t>(ar);
		}

		void swap(basic_response_header& other)
		{
			basic_header::swap(other);

			std::swap(result, other.result);
		}

	public:
		uint32_t result;
	};
} // namespace aquarius

std::ostream& operator<<(std::ostream& os, const aquarius::basic_request_header& other)
{
	other << os;

	return os;
}

std::ostream& operator<<(std::ostream& os, const aquarius::basic_response_header& other)
{
	other << os;

	return os;
}