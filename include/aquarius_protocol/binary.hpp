#pragma once
#include <aquarius_protocol/archive.hpp>
#include <aquarius_protocol/concepts.hpp>
#include <aquarius_protocol/reflection.hpp>
#include <string>
#include <vector>

namespace aquarius
{
	struct binary
	{
		using archive_type = archive;

		using value_type = archive_type::char_type;

		inline constexpr static std::size_t value_size = sizeof(value_type);

		template <boolean_t T>
		static void to(archive_type& ar, const T& value)
		{
			uint8_t temp{};

			value ? temp = 1 : temp = 0;

			ar.sputn((value_type*)&temp, sizeof(uint8_t));
		}

		template <uinteger_t T>
		static void to(archive_type& ar, const T& value)
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

		template <integer_t T>
		static void to(archive_type& ar, const T& value)
		{
			auto temp = (value << 1) ^ (value >> (sizeof(T) * 8 - 1));

			return to<uint32_t>(ar, static_cast<uint32_t>(temp));
		}

		template <repeated_t T>
		static void to(archive_type& ar, const T& value)
		{
			to(ar, static_cast<uint64_t>(value.size()));

			for (auto& c : value)
			{
				to(ar, c);
			}
		}

		template <string_t T>
		static void to(archive_type& ar, const T& value)
		{
			to(ar, static_cast<uint64_t>(value.size()));

			ar.sputn((value_type*)value.data(), value.size());
		}

		template <float_t T>
		static void to(archive_type& ar, const T& value)
		{
			ar.sputn((value_type*)&value, sizeof(T));
		}

		template <reflactable T>
		static void to(archive_type& ar, const T& value)
		{
			auto to_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ (to(ar, boost::pfr::get<I, T>(value)), ...); };

			to_binary_impl(std::make_index_sequence<member_count<T>()>{});
		}

		template <uinteger_t T>
		static auto from(archive_type& ar) -> T
		{
			using convert_type = typename convert_stream_value_type<value_type>::type;
			convert_type c{};
			ar.sgetn((value_type*)&c, value_size);

			T value = static_cast<convert_type>(c);
			if (value >= 0x80)
			{
				value -= 0x80;

				int8_t temp_bit = 7;

				while (ar.sgetn((value_type*)&c, value_size), (c & 0x80) != 0)
				{
					value += ((static_cast<T>(c) & 0x7f) << temp_bit);

					temp_bit += 7;
				}

				value += (static_cast<T>(c) << temp_bit);
			}

			return value;
		}

		template <integer_t T>
		static auto from(archive_type& ar) -> T
		{
			T value = static_cast<T>(from<uint64_t>(ar));

			return (value >> 1) ^ (~(value & 1) + 1);
		}

		template <float_t T>
		static auto from(archive_type& ar) -> T
		{
			T value;

			ar.sgetn((value_type*)&value, sizeof(T));

			return value;
		}

		template <repeated_t T>
		static auto from(archive_type& ar) -> T
		{
			T value{};
			std::size_t size = from<std::size_t>(ar);

			if (size == 0)
				return value;

			value.resize(size);

			for (int i = 0; i < size; ++i)
			{
				value[i] = from<typename T::value_type>(ar);
			}

			return value;
		}

		template <string_t T>
		static auto from(archive_type& ar) -> T
		{
			T value{};

			using value_type = archive_type::char_type;

			std::size_t size = from<std::size_t>(ar);

			value.resize(size);

			ar.sgetn((value_type*)value.data(), size);

			return value;
		}

		template <boolean_t T>
		static auto from(archive_type& ar) -> T
		{
			T value{};

			ar.sgetn((value_type*)&value, value_size);

			return value;
		}

		template <reflactable T>
		static auto from(archive_type& ar) -> T
		{
			auto from_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ return T{ from<element_t<I, T>>(ar)... }; };

			return from_binary_impl(std::make_index_sequence<member_count<T>()>{});
		}
	};
} // namespace aquarius