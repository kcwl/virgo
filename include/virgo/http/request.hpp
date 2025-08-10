#pragma once
#include <virgo/detail/config.hpp>
#include <virgo/http/detail/basic_message.hpp>

namespace virgo
{
	namespace http
	{
		template <typename Body>
		using request = detail::basic_message<true, Body>;
	}
} // namespace virgo