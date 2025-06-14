#pragma once
#include <algorithm>
#include <aquarius_protocol/binary.hpp>
#include <cstdint>
#include <ostream>
#include <vector>

namespace aquarius
{
	namespace detail
	{
		class header_base
		{
		public:
			header_base()
				: uuid_(0)
			{}

			header_base(const header_base& other)
				: uuid_(other.uuid_)
			{}

			header_base(header_base&& other) noexcept
				: uuid_(std::exchange(other.uuid_, 0))
			{}

			header_base& operator=(const header_base& other)
			{
				if (this != std::addressof(other))
				{
					uuid_ = other.uuid_;
				}

				return *this;
			}

			header_base& operator=(header_base&& other) noexcept
			{
				if (this != std::addressof(other))
				{
					uuid_ = std::exchange(other.uuid_, 0);
				}

				return *this;
			}

		public:
			bool operator==(const header_base& other) const
			{
				return uuid_ == other.uuid_;
			}

			std::ostream& operator<<(std::ostream& os) const
			{
				os << "header base: " << uuid_ << "\t";

				return os;
			}

		public:
			void pack(std::vector<char>& buff)
			{
				serialize::to_binary(uuid_, buff);
			}

			void unpack(std::vector<char>& buff)
			{
				uuid_ = serialize::from_binary<uint64_t>(buff);
			}

		public:
			uint64_t uuid_;
		};
	} // namespace detail
} // namespace aquarius
