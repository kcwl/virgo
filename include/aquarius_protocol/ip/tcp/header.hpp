#pragma once
#include <algorithm>
#include <aquarius_protocol/binary.hpp>
#include <cstdint>
#include <ostream>

namespace aquarius
{
	namespace ip
	{
		namespace tcp
		{
			class header_base
			{
			public:
				bool operator==(const header_base& other) const
				{
					return crc32 == other.crc32 && timestamp == other.timestamp;
				}

				std::ostream& operator<<(std::ostream& os) const
				{
					os << "header: " << crc32 << "\t" << timestamp << "\t";

					return os;
				}

			public:
				template<typename Buffer>
				void to_binary(Buffer& ar)
				{
					binary::to(ar, crc32);
					binary::to(ar, timestamp);
				}

				template<typename Buffer>
				void from_binary(Buffer& ar)
				{
					crc32 = binary::from<uint32_t>(ar);
					timestamp = binary::from<int64_t>(ar);
				}

				void swap(header_base& other)
				{
					std::swap(crc32, other.crc32);
					std::swap(timestamp, other.timestamp);
				}

			public:
				uint32_t crc32;

				int64_t timestamp;
			};

			class request_header : public header_base
			{
				using base_type = header_base;

			public:
				bool operator==(const request_header& other) const
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
				template<typename Buffer>
				void to_binary(Buffer& ar)
				{
					base_type::to_binary(ar);

					binary::to(ar, type);
				}

				template<typename Buffer>
				void from_binary(Buffer& ar)
				{
					base_type::from_binary(ar);

					type = binary::from<uint32_t>(ar);
				}

				void swap(request_header& other)
				{
					header_base::swap(other);

					std::swap(type, other.type);
				}

			public:
				uint32_t type;
			};

			class response_header : public header_base
			{
				using base_type = header_base;

			public:
				bool operator==(const response_header& other) const
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
				template<typename Buffer>
				void to_binary(Buffer& ar)
				{
					base_type::to_binary(ar);

					binary::to(ar, result);
				}

				template<typename Buffer>
				void from_binary(Buffer& ar)
				{
					base_type::from_binary(ar);

					result = binary::from<uint32_t>(ar);
				}

				void swap(response_header& other)
				{
					header_base::swap(other);

					std::swap(result, other.result);
				}

			public:
				uint32_t result;
			};
		} // namespace tcp
	} // namespace ip
} // namespace aquarius

std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::request_header& other)
{
	other << os;

	return os;
}

std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::response_header& other)
{
	other << os;

	return os;
}