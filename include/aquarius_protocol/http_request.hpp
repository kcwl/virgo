#pragma once
#include <aquarius_protocol/basic_http_message.hpp>

namespace aquarius
{
	template <typename Body>
	using http_request = basic_http_message<true, Body>;
}