#pragma once
#include <cstdint>
#include <ostream>
#include <span>
#include <aquarius_protocol/binary.hpp>

namespace aquarius
{
	template <bool Request>
	class basic_header
	{
	public:
		constexpr static bool is_request = Request;

	public:
		basic_header() = default;

		virtual~basic_header() = default;

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			os << version_ << "\t";

			return os;
		}

	public:
		void version(uint32_t v)
		{
			version_ = v;
		}

		uint32_t version() const
		{
			return version_;
		}

		template <typename BuffSequence>
		void unpack(BuffSequence& buffer)
		{
			version_ = serialize::from_binary<uint32_t>(buffer);
		}

		template <typename BufferSequence>
		void pack(BufferSequence& buffer)
		{
			serialize::to_binary(version_, buffer);
		}

	private:
		uint32_t version_;
	};
} // namespace aquarius