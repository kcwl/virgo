#pragma once
#include <aquarius_protocol/basic_message.hpp>
#include <aquarius_protocol/ip/tcp/header.hpp>

namespace aquarius
{
	namespace ip
	{
		namespace tcp
		{
			template <typename Body, std::size_t Number>
			class request : public basic_message<request_header, Body, Number>
			{
				using base_type = basic_message<request_header, Body, Number>;
			public:
				static constexpr std::size_t proto = Number;

			public:
				request() = default;

			public:
				bool operator==(const request& other) const
				{
					return base_type::operator==(other);
				}

				std::ostream& operator<<(std::ostream& os) const
				{
					base_type::operator<<(os);

					return os;
				}
			};

			template <typename Body, std::size_t Number>
			class response : public basic_message<response_header, Body, Number>
			{
				using base_type = basic_message<response_header, Body, Number>;
			public:
				static constexpr std::size_t proto = Number;

			public:
				response() = default;

			public:
				bool operator==(const response& other) const
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

//template <typename Body, std::size_t Number>
//std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::request<Body, Number>& other)
//{
//	other << os;
//
//	return os;
//}
//
//template <typename Body, std::size_t Number>
//std::ostream& operator<<(std::ostream& os, const aquarius::ip::tcp::response<Body, Number>& other)
//{
//	other << os;
//
//	return os;
//}