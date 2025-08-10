#pragma once
#include <virgo/detail/config.hpp>
#include <virgo/http/detail/basic_message.hpp>

namespace virgo
{
	namespace http
	{
		template <typename Body>
		using response = detail::basic_message<false, Body>;
	}
} // namespace virgo