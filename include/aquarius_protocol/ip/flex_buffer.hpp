#pragma once
#include <aquarius_protocol/basic_flex_buffer.hpp>
#include <string>

namespace aquarius
{
	using flex_buffer = basic_flex_buffer<uint8_t, std::char_traits<uint8_t>>;
}