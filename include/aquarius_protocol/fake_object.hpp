#pragma once

namespace aquarius
{
	namespace detail
	{
		template <typename T>
		struct wrapper
		{
			const T value;
		};

		template <typename T>
		extern const wrapper<T> do_not_use_undefined_value;

		template <typename T>
		constexpr const T& fake_object() noexcept
		{
			return do_not_use_undefined_value<T>.value;
		}
	} // namespace detail
} // namespace aquarius