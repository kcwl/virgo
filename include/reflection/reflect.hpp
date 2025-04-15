#pragma once
#include <array>
#include <string_view>
#include <concepts>

using namespace std::string_view_literals;

namespace aquarius
{
	template <typename _Ty>
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

	template <typename _Ty>
	concept reflactable = requires { typename reflect<std::remove_cvref_t<_Ty>>::value_type; };
} // namespace aquarius