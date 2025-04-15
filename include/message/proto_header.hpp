#pragma once
#include <cstdint>
#include <map>

namespace aquarius
{
	class basic_header
	{
	public:
		uint32_t total_length_;

		uint32_t compress_length_;

		int32_t type_;

		uint32_t crc32_;
	};

	class basic_super_header : public basic_header
	{
	public:
		uint32_t total_package_;

		uint32_t pack_seq_;
	};

	class basic_distribute_header : public basic_super_header
	{
	public:
		uint32_t sender_;

		uint32_t receiver_;
	};
}
