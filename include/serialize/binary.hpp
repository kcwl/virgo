#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <limits>
#include <reflection.hpp>


namespace aquarius
{
	template <typename _Ty, typename... _Args>
	concept disjunction_same_as = std::disjunction_v<std::is_same<std::remove_cvref_t<_Ty>, _Args>...>;

	template <typename _Ty>
	concept integer_t = disjunction_same_as<_Ty, int8_t, int16_t, int32_t>;

	template <typename _Ty>
	concept uinteger_t = disjunction_same_as<_Ty, uint8_t, uint16_t, uint32_t>;

	template <typename _Ty>
	concept integer64_t = std::is_same_v<std::remove_cvref_t<_Ty>, int64_t>;

	template <typename _Ty>
	concept uinteger64_t = std::is_same_v<std::remove_cvref_t<_Ty>, uint64_t>;

	template <typename _Ty>
	concept all_integer_t = integer_t<_Ty> || integer64_t<_Ty>;

	template <typename _Ty>
	concept all_uinteger_t = uinteger_t<_Ty> || uinteger64_t<_Ty>;

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

	template<typename _Ty>
	concept boolean_t = std::same_as<std::remove_cvref_t<_Ty>, bool>;

	template <typename _Ty>
	concept archive_t = requires(_Ty value) {
		std::begin(value);
		std::end(value);
		value.put(uint8_t{});
		value.get();
		value.fill(int32_t{}, uint8_t{});

		value.template get<class T>(std::declval<T&>());
	};

	template<archive_t _Archive, boolean_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		uint8_t temp{};

		value ? temp = 1 : temp = 0;

		ar.put(temp);
	}

	template <archive_t _Archive, uinteger_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		auto temp = value;

		while (temp >= 0x80)
		{
			ar.put(static_cast<uint8_t>(temp | 0x80));

			temp >>= 7;
		}

		ar.put(static_cast<uint8_t>(temp));
	}

	template <archive_t _Archive, integer_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		auto temp = (value << 1) ^ (value >> (sizeof(_Ty) * 8 - 1));

		return to_binary<_Archive, uint32_t>(ar, static_cast<uint32_t>(temp));
	}

	template <archive_t _Archive, uinteger64_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		_Ty temp = value;
		if (value > (std::numeric_limits<uint32_t>::max)())
		{
			ar.fill(4, 0x80);

			temp = value - (std::numeric_limits<uint32_t>::max)();
		}

		return to_binary(ar, static_cast<uint32_t>(temp));
	}

	template <archive_t _Archive, integer64_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		auto temp = (value << 1) ^ (value >> (sizeof(_Ty) * 8 - 1));

		return to_binary(ar, static_cast<uint64_t>(temp));
	}

	template <archive_t _Archive, repeated_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		to_binary(ar, static_cast<uint64_t>(value.size()));

		for (auto& c : value)
		{
			to_binary(ar, static_cast<int8_t>(c));
		}
	}

	template <archive_t _Archive, string_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		to_binary(ar, static_cast<uint64_t>(value.size()));

		//std::copy(value.begin(), value.end(), std::back_inserter(ar));
	}

	template <archive_t _Archive, float_t _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		//std::copy(reinterpret_cast<const uint8_t*>(&value), reinterpret_cast<const uint8_t*>(&value) + sizeof(value),
		//		  std::back_inserter(ar));
	}

	template <archive_t _Archive, reflactable _Ty>
	void to_binary(_Archive& ar, const _Ty& value)
	{
		auto to_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
		{
			(to_binary(ar, std::get<I>(member_bind_helper<_Ty, member_count<_Ty>()>::values(value))),
			 ...);
		};

		to_binary_impl(std::make_index_sequence<member_count<_Ty>()>{});
	}

	template <all_uinteger_t _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar) -> _Ty
	{
		uint8_t c = ar.get();

		_Ty value = c;
		if (value >= 0x80)
		{
			value -= 0x80;

			int8_t temp_bit = 7;

			while (c = ar.get(), (c & 0x80) != 0)
			{
				c &= 0x7f;
				value += ((c & 0x7f) << temp_bit);

				temp_bit += 7;
			}

			value += (c << temp_bit);
		}

		return value;
	}

	template <all_integer_t _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar) -> _Ty
	{
		_Ty value = from_binary<_Ty>(ar);

		return (value >> 1) ^ (~(value & 1) + 1);
	}

	template <float_t _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar) -> _Ty
	{
		return true;
	}

	template <repeated_t _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar) -> _Ty
	{
		_Ty value{};
		std::size_t size = from_binary<std::size_t>(ar);

		if (size == 0)
			return value;

		value.resize(size);

		for (int i = 0; i < size; ++i)
		{
			value[i] = from_binary<_Ty>(ar);
		}

		return value;
	}

	template <string_t _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar) -> _Ty
	{
		_Ty value{};

		std::size_t size = from_binary<std::size_t>(ar);

		value.resize(size);

		//std::copy(ar.rbegin(), ar.rbegin() + size, value.begin());

		return value;
	}

	template <reflactable _Ty, archive_t _Archive>
	auto from_binary(_Archive& ar)
	{
		auto from_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
		{ return _Ty{ from_binary<element_t<I, _Ty>>(ar)... }; };

		return from_binary_impl(std::make_index_sequence<member_count<_Ty>()>{});
	}
} // namespace aquarius