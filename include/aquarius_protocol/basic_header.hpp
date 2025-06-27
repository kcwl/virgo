#pragma once
#include <cstdint>
#include <ostream>

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

		std::ostream& operator<<(std::ostream& os)
		{
			os << version_;

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

	private:
		uint32_t version_;
	};
} // namespace aquarius