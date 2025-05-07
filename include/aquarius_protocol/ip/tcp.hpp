#pragma once
#include <aquarius_protocol/basic_message.hpp>

namespace aquarius
{
	namespace ip
	{
		namespace tcp
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

		} // namespace tcp
	} // namespace ip
} // namespace aquarius

template <aquarius::standard_layout_t Body, std::size_t Number>
std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::basic_request<Body, Number>& other)
{
	other << os;

	return os;
}

template <aquarius::standard_layout_t Body, std::size_t Number>
std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::basic_response<Body, Number>& other)
{
	other << os;

	return os;
}