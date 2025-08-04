#pragma once
#include <aquarius_protocol/basic_tcp_message.hpp>
#include <aquarius_protocol/custom_tcp_header.hpp>

namespace aquarius
{
	template<typename Body, typename Header = custom_tcp_request_header>
	using tcp_request = basic_tcp_message<Header, Body>;
}