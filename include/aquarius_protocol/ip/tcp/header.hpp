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
				constexpr static std::size_t header_static_size = 8;

			public:
				header_base()
					: crc32_(0)
					, timestamp_(0)
				{}

				header_base(const header_base& other)
					: crc32_(other.crc32_)
					, timestamp_(other.timestamp_)
				{}

				header_base(header_base&& other)
					: crc32_(std::exchange(other.crc32_, 0))
					, timestamp_(std::exchange(other.timestamp_, 0))
				{}

				header_base& operator=(const header_base& other)
				{
					if (this != std::addressof(other))
					{
						crc32_ = other.crc32_;

						timestamp_ = other.timestamp_;
					}

					return *this;
				}

				header_base& operator=(header_base&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						crc32_ = std::exchange(other.crc32_, 0);

						timestamp_ = std::exchange(other.timestamp_, 0);
					}

					return *this;
				}

			public:
				bool operator==(const header_base& other) const
				{
					return crc32_ == other.crc32_ && timestamp_ == other.timestamp_;
				}

				std::ostream& operator<<(std::ostream& os) const
				{
					os << "header: " << crc32_ << "\t" << timestamp_ << "\t";

					return os;
				}

			public:
				template <typename Buffer>
				void to_binary(Buffer& ar)
				{
					binary::to(ar, crc32_);
					binary::to(ar, timestamp_);
				}

				template <typename Buffer>
				void from_binary(Buffer& ar)
				{
					crc32_ = binary::from<uint32_t>(ar);
					timestamp_ = binary::from<int64_t>(ar);
				}

			public:
				uint32_t crc32_;

				int64_t timestamp_;

				uint64_t uuid_;
			};

			class request_header : public header_base
			{
				using base_type = header_base;

			public:
				request_header()
					: type()
				{}

				request_header(const request_header& other)
					: base_type(other)
					, type(other.type)
				{}

				request_header(request_header&& other)
					: base_type(std::move(other))
					, type(std::exchange(other.type, 0))
				{}

				request_header& operator=(const request_header& other)
				{
					if (this != std::addressof(other))
					{
						base_type::operator=(other);

						type = other.type;
					}

					return *this;
				}

				request_header& operator=(request_header&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						base_type::operator=(std::move(other));

						type = std::exchange(other.type, 0);
					}

					return *this;
				}

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
				template <typename Buffer>
				void to_binary(Buffer& ar)
				{
					base_type::to_binary(ar);

					binary::to(ar, type);
				}

				template <typename Buffer>
				void from_binary(Buffer& ar)
				{
					base_type::from_binary(ar);

					type = binary::from<uint32_t>(ar);
				}

			public:
				uint32_t type;
			};

			class response_header : public header_base
			{
				using base_type = header_base;
			public:
				response_header()
					: result_()
				{
				}

				response_header(const response_header& other)
					: base_type(other)
					, result_(other.result_)
				{
				}

				response_header(response_header&& other)
					: base_type(std::move(other))
					, result_(std::exchange(other.result_, 0))
				{
				}

				response_header& operator=(const response_header& other)
				{
					if (this != std::addressof(other))
					{
						base_type::operator=(other);

						result_ = other.result_;
					}

					return *this;
				}

				response_header& operator=(response_header&& other) noexcept
				{
					if (this != std::addressof(other))
					{
						base_type::operator=(std::move(other));

						result_ = std::exchange(other.result_, 0);
					}

					return *this;
				}
			public:
				bool operator==(const response_header& other) const
				{
					return base_type::operator==(other) && result_ == other.result_;
				}

				std::ostream& operator<<(std::ostream& os) const
				{
					base_type::operator<<(os);

					os << result_;

					return os;
				}

			public:
				template <typename Buffer>
				void to_binary(Buffer& ar)
				{
					base_type::to_binary(ar);

					binary::to(ar, result_);
				}

				template <typename Buffer>
				void from_binary(Buffer& ar)
				{
					base_type::from_binary(ar);

					result_ = binary::from<uint32_t>(ar);
				}

			public:
				uint32_t result_;
			};
		} // namespace tcp
	} // namespace ip
} // namespace aquarius

namespace std
{
	inline std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::request_header& other)
	{
		other << os;

		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::response_header& other)
	{
		other << os;

		return os;
	}
} // namespace std
