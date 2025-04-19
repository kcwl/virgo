#pragma once
#include <message/basic_message.hpp>

namespace aquarius
{
	template <standard_layout_t Body, std::size_t Number>
	class basic_response : public basic_message<basic_response_header, Body, Number>
	{
		using base_type = basic_message<basic_request_header, Body, Number>;

	public:
		basic_response() = default;

	public:
		bool operator==(const basic_response& other) const
		{
			return base_type::operator==(other);
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			return os;
		}
	};
} // namespace aquarius

template<aquarius::standard_layout_t Body, std::size_t Number>
std::ostream& operator<<(std::ostream& os, const aquarius::basic_response<Body, Number>& other)
{
	other << os;

	return os;
}