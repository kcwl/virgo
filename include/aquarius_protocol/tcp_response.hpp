#pragma once
#include <aquarius_protocol/basic_tcp_message.hpp>

namespace aquarius
{
	template <typename Body, typename Header = custom_tcp_response_header>
	using tcp_response = basic_tcp_message<Header, Body>;
}