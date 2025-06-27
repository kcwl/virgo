#pragma once
#include <aquarius_protocol/basic_tcp_message.hpp>

namespace aquarius
{
	template<typename Body>
	using tcp_request = basic_tcp_message<true, Body>;
}