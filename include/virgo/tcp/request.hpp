#pragma once
#include <virgo/detail/config.hpp>
#include <virgo/tcp/detail/basic_message.hpp>
#include <virgo/tcp/detail/custom_header.hpp>

namespace virgo
{
	namespace tcp
	{
		template <typename Body, typename Header = detail::custom_request_header>
		using request = detail::basic_message<Header, Body>;
	}
} // namespace virgo