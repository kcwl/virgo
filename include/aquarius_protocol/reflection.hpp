#pragma once
#include <aquarius_protocol/reflect_helper.hpp>
#include <boost/pfr.hpp>

namespace aquarius
{
	template <std::size_t I, typename T>
	using element_t = boost::pfr::tuple_element_t<I, T>;

	template <typename T>
	constexpr auto name()
	{
		return reflect<T>::topic();
	}

	template <std::size_t I, typename T>
	constexpr auto field()
	{
		return std::get<I>(reflect<T>::fields());
	}

	template <typename T>
	constexpr auto member_count()
	{
		return reflect<T>::fields().size();
	}
} // namespace aquarius