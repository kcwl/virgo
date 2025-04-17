#pragma once
#include <algorithm>
#include <cstdint>
#include <serialize.hpp>

namespace aquarius
{
	class basic_header
	{
	public:
		//template <archive_t _Archive>
		template <typename _Archive>
		void to_binary(_Archive& ar)
		{
			binary<_Archive>::template to(ar, total_length);
			binary<_Archive>::template to(ar, compress_length);
			binary<_Archive>::template to(ar, crc32);
			binary<_Archive>::template to(ar, total_package);
			binary<_Archive>::template to(ar, pack_seq);
		}

		template <archive_t _Archive>
		void from_binary(_Archive& ar)
		{
			total_length = binary<_Archive>::template from<uint32_t>(ar);
			compress_length = binary<_Archive>::template from<uint32_t>(ar);
			crc32 = binary<_Archive>::template from<uint32_t>(ar);
			total_package = binary<_Archive>::template from<uint32_t>(ar);
			pack_seq = binary<_Archive>::template from<uint32_t>(ar);
		}

		void swap(basic_header& other)
		{
			std::swap(total_length, other.total_length);
			std::swap(compress_length, other.compress_length);
			std::swap(crc32, other.crc32);
			std::swap(total_package, other.total_package);
			std::swap(pack_seq, other.pack_seq);
		}

	public:
		uint32_t total_length;

		uint32_t compress_length;

		uint32_t crc32;

		uint32_t total_package;

		uint32_t pack_seq;
	};

	class basic_request_header : public basic_header
	{
		using base_type = basic_header;

	public:
		template <archive_t _Archive>
		void to_binary(_Archive& ar)
		{
			base_type::to_binary(ar);

			binary<_Archive>::to(ar, type);
		}

		template <archive_t _Archive>
		void from_binary(_Archive& ar)
		{
			base_type::from_binary(ar);

			type = binary<_Archive>::template from<uint32_t>(ar);
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
		template <archive_t _Archive>
		void to_binary(_Archive& ar)
		{
			base_type::to_binary(ar);

			binary<_Archive>::to(ar, result);
		}

		template <archive_t _Archive>
		void from_binary(_Archive& ar)
		{
			base_type::from_binary(ar);

			result = binary<_Archive>::template from<uint32_t>(ar);
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
