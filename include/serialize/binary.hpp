#pragma once
#include <reflection.hpp>
#include <string>
#include <vector>
#include <archive.hpp>

namespace aquarius
{
	template <typename _Ty, typename... _Args>
	concept disjunction_same_as = std::disjunction_v<std::is_same<std::remove_cvref_t<_Ty>, _Args>...>;

	template <typename _Ty>
	concept integer_t = disjunction_same_as<_Ty, int8_t, int16_t, int32_t, int64_t>;

	template <typename _Ty>
	concept uinteger_t = disjunction_same_as<_Ty, uint8_t, uint16_t, uint32_t, uint64_t>;

	template <typename _Ty>
	concept float_t = disjunction_same_as<std::remove_cvref_t<_Ty>, float, double>;

	template <typename _Ty>
	struct is_vector : std::false_type
	{};

	template <typename _Ty, typename _Alloc>
	struct is_vector<std::vector<_Ty, _Alloc>> : public std::true_type
	{};

	template <typename _Ty>
	struct is_string : std::false_type
	{};

	template <>
	struct is_string<std::string> : public std::true_type
	{};

	template <typename _Ty>
	concept string_t = is_string<std::remove_cvref_t<_Ty>>::value;

	template <typename _Ty>
	concept repeated_t = is_vector<std::remove_cvref_t<_Ty>>::value;

	template <typename _Ty>
	concept boolean_t = std::same_as<std::remove_cvref_t<_Ty>, bool>;

	template<typename _Ty>
	struct convert_stream_value_type
	{
		using type = _Ty;
	};

	template<>
	struct convert_stream_value_type<char>
	{
		using type = uint8_t;
	};

	struct binary
	{
		using archive_type = archive;

		using value_type = archive_type::char_type;

		inline constexpr static std::size_t value_size = sizeof(value_type);

		template <boolean_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			uint8_t temp{};

			value ? temp = 1 : temp = 0;

			ar.sputn((value_type*)&temp, sizeof(uint8_t));
		}

		template <uinteger_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			auto temp = value;

			value_type c{};

			while (temp >= 0x80)
			{
				c = static_cast<value_type>(temp | 0x80);
				ar.sputn(&c, value_size);

				temp >>= 7;
			}
			c = static_cast<value_type>(temp);
			ar.sputn(&c, value_size);
		}

		template <integer_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			auto temp = (value << 1) ^ (value >> (sizeof(_Ty) * 8 - 1));

			return to<uint32_t>(ar, static_cast<uint32_t>(temp));
		}

		template <repeated_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			to(ar, static_cast<uint64_t>(value.size()));

			for (auto& c : value)
			{
				to(ar, c);
			}
		}

		template <string_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			to(ar, static_cast<uint64_t>(value.size()));

			ar.sputn((value_type*)value.data(), value.size());
		}

		template <float_t _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			ar.sputn((value_type*)&value, sizeof(_Ty));
		}

		template <reflactable _Ty>
		static void to(archive_type& ar, const _Ty& value)
		{
			auto to_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ (to(ar, std::get<I>(member_bind_helper<_Ty, member_count<_Ty>()>::values(value))), ...); };

			to_binary_impl(std::make_index_sequence<member_count<_Ty>()>{});
		}

		template <uinteger_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			using convert_type = typename convert_stream_value_type<value_type>::type;
			convert_type c{};
			ar.sgetn((value_type*)&c, value_size);

			_Ty value = static_cast<convert_type>(c);
			if (value >= 0x80)
			{
				value -= 0x80;

				int8_t temp_bit = 7;

				while (ar.sgetn((value_type*)&c, value_size), (c & 0x80) != 0)
				{
					value += ((static_cast<_Ty>(c) & 0x7f) << temp_bit);

					temp_bit += 7;
				}

				value += (static_cast<_Ty>(c) << temp_bit);
			}

			return value;
		}

		template <integer_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			_Ty value = static_cast<_Ty>(from<uint64_t>(ar));

			return (value >> 1) ^ (~(value & 1) + 1);
		}

		template <float_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			_Ty value;

			ar.sgetn((value_type*)&value, sizeof(_Ty));

			return value;
		}

		template <repeated_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			_Ty value{};
			std::size_t size = from<std::size_t>(ar);

			if (size == 0)
				return value;

			value.resize(size);

			for (int i = 0; i < size; ++i)
			{
				value[i] = from<typename _Ty::value_type>(ar);
			}

			return value;
		}

		template <string_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			_Ty value{};

			using value_type = archive_type::char_type;

			std::size_t size = from<std::size_t>(ar);

			value.resize(size);

			ar.sgetn((value_type*)value.data(), size);

			return value;
		}

		template <boolean_t _Ty>
		static auto from(archive_type& ar) -> _Ty
		{
			_Ty value{};

			ar.sgetn((value_type*)&value, value_size);

			return value;
		}

		template <reflactable _Ty>
		static auto from(archive_type& ar)
		{
			auto from_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ return _Ty{ from<element_t<I, _Ty>>(ar)... }; };

			return from_binary_impl(std::make_index_sequence<member_count<_Ty>()>{});
		}
	};
} // namespace aquarius