#pragma once
#include <array>
#include <string_view>
#include <concepts>

using namespace std::string_view_literals;

namespace aquarius
{
	template <typename T>
	struct reflect
	{
		constexpr static std::string_view topic()
		{
			return {};
		}

		constexpr static std::array<std::string_view, 0> fields()
		{
			return {};
		}
	};

	template <typename T>
	concept reflactable = requires { typename reflect<std::remove_cvref_t<T>>::value_type; };
} // namespace aquarius