#pragma once
#include <detail/basic_message.hpp>

namespace aquarius
{
	template <standard_layout_t Body, std::size_t Number>
	class basic_request : public basic_message<basic_request_header, Body, Number>
	{
		using base_type = basic_message<basic_request_header, Body, Number>;

	public:
		basic_request() = default;

	public:
		bool operator==(const basic_request& other) const
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
std::ostream& operator<<(std::ostream& os, const aquarius::basic_request<Body, Number>& other)
{
	other << os;

	return os;
}