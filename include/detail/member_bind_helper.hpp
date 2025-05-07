#pragma once
#include <detail/fake_object.hpp>
#include <tuple>

namespace aquarius
{
	template <typename T>
	struct wrapper
	{
		const T Value;
	};

	template <typename T>
	extern const wrapper<T> do_not_use_undefined_value;

	template <typename T>
	constexpr const T& fake_object() noexcept
	{
		return do_not_use_undefined_value<T>.Value;
	}

	template <typename T, std::size_t I>
	struct member_bind_helper
	{
		static_assert(I > 32 || I < 0, "reflection member must be less or equal than 32!");
	};

	template <typename T>
	struct member_bind_helper<T, 0>
	{
		static constexpr auto empty_values()
		{
			return std::make_tuple();
		}

		template <T Value>
		static constexpr auto static_values()
		{
			return std::make_tuple();
		}

		static constexpr auto values(const T& Value)
		{
			return std::make_tuple();
		}
	};

#define REFLECT_MEMBER_BIND_TO_TUPLE(N, ...)                                                                           \
	template <typename T>                                                                                              \
	struct member_bind_helper<T, N>                                                                                    \
	{                                                                                                                  \
		static constexpr auto empty_values()                                                                           \
		{                                                                                                              \
			auto& [__VA_ARGS__] = fake_object<T>();                                                                    \
			auto f = [](auto&... args) { return std::make_tuple(args...); };                                           \
			return std::apply(f, std::tie(__VA_ARGS__));                                                               \
		}                                                                                                              \
		template <auto Value>                                                                                          \
		static constexpr auto static_values()                                                                          \
		{                                                                                                              \
			auto&& [__VA_ARGS__] = Value;                                                                              \
			return std::make_tuple(__VA_ARGS__);                                                                       \
		}                                                                                                              \
		static constexpr auto values(const T& Value)                                                                   \
		{                                                                                                              \
			auto& [__VA_ARGS__] = Value;                                                                               \
			return std::make_tuple(__VA_ARGS__);                                                                       \
		}                                                                                                              \
	};

	REFLECT_MEMBER_BIND_TO_TUPLE(1, f1);
	REFLECT_MEMBER_BIND_TO_TUPLE(2, f1, f2);
	REFLECT_MEMBER_BIND_TO_TUPLE(3, f1, f2, f3);
	REFLECT_MEMBER_BIND_TO_TUPLE(4, f1, f2, f3, f4);
	REFLECT_MEMBER_BIND_TO_TUPLE(5, f1, f2, f3, f4, f5);
	REFLECT_MEMBER_BIND_TO_TUPLE(6, f1, f2, f3, f4, f5, f6);
	REFLECT_MEMBER_BIND_TO_TUPLE(7, f1, f2, f3, f4, f5, f6, f7);
	REFLECT_MEMBER_BIND_TO_TUPLE(8, f1, f2, f3, f4, f5, f6, f7, f8);
	REFLECT_MEMBER_BIND_TO_TUPLE(9, f1, f2, f3, f4, f5, f6, f7, f8, f9);
	REFLECT_MEMBER_BIND_TO_TUPLE(10, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10);
	REFLECT_MEMBER_BIND_TO_TUPLE(11, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11);
	REFLECT_MEMBER_BIND_TO_TUPLE(12, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12);
	REFLECT_MEMBER_BIND_TO_TUPLE(13, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13);
	REFLECT_MEMBER_BIND_TO_TUPLE(14, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14);
	REFLECT_MEMBER_BIND_TO_TUPLE(15, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15);
	REFLECT_MEMBER_BIND_TO_TUPLE(16, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16);
	REFLECT_MEMBER_BIND_TO_TUPLE(17, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17);
	REFLECT_MEMBER_BIND_TO_TUPLE(18, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18);
	REFLECT_MEMBER_BIND_TO_TUPLE(19, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19);
	REFLECT_MEMBER_BIND_TO_TUPLE(20, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20);
	REFLECT_MEMBER_BIND_TO_TUPLE(21, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21);
	REFLECT_MEMBER_BIND_TO_TUPLE(22, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22);
	REFLECT_MEMBER_BIND_TO_TUPLE(23, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23);
	REFLECT_MEMBER_BIND_TO_TUPLE(24, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24);
	REFLECT_MEMBER_BIND_TO_TUPLE(25, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25);
	REFLECT_MEMBER_BIND_TO_TUPLE(26, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26);
	REFLECT_MEMBER_BIND_TO_TUPLE(27, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27);
	REFLECT_MEMBER_BIND_TO_TUPLE(28, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27, f28);
	REFLECT_MEMBER_BIND_TO_TUPLE(29, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29);
	REFLECT_MEMBER_BIND_TO_TUPLE(30, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30);
	REFLECT_MEMBER_BIND_TO_TUPLE(31, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31);
	REFLECT_MEMBER_BIND_TO_TUPLE(32, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18,
								 f19, f20, f21, f22, f23, f24, f25, f26, f27, f28, f29, f30, f31, f32);
} // namespace aquarius