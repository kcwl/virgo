#pragma once

namespace aquarius
{
	namespace detail
	{
		template <typename _Ty>
		struct wrapper
		{
			const _Ty value;
		};

		template <typename _Ty>
		extern const wrapper<_Ty> do_not_use_undefined_value;

		template <typename _Ty>
		constexpr const _Ty& fake_object() noexcept
		{
			return do_not_use_undefined_value<_Ty>.value;
		}
	} // namespace detail
} // namespace aquarius