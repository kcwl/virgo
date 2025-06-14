#pragma once
#include <aquarius_protocol/concepts.hpp>
#include <aquarius_protocol/reflect_helper.hpp>
#include <boost/pfr.hpp>
#include <span>

namespace aquarius
{
	namespace serialize
	{
		template <integer_t T, typename BufferSequence>
		inline void to_binary(const T& value, BufferSequence& buff)
		{
			auto temp = value;

			while (temp >= 0x80)
			{
				buff.push_back(static_cast<char>(temp | 0x80));

				temp >>= 7;
			}
			buff.push_back(static_cast<char>(temp));
		}

		template <zig_zag T, typename BufferSequence>
		inline void to_binary(const T& value, BufferSequence& buff)
		{
			auto temp = (value << 1) ^ (value >> (sizeof(T) * 8 - 1));

			return to_binary<uint64_t>(temp, buff);
		}

		template <typename T, typename BufferSequence>
		inline void to_binary(const T& value, BufferSequence& buff)
		{
			auto begin = (char*)&value;
			auto end = (char*)&value + sizeof(T);
			std::copy(begin, end, std::back_inserter(buff));
		}

		template <repeated_t T, typename BufferSequence>
		static void to_binary(const T& value, BufferSequence& buff)
		{
			to_binary(value.size(), buff);

			for (auto& c : value)
			{
				to_binary(c, buff);
			}
		}

		template <string_t T, typename BufferSequence>
		static void to_binary(const T& value, BufferSequence& buff)
		{
			to_binary(value.size(), buff);

			std::copy(std::begin(value), std::end(value), std::back_inserter(buff));
		}

		template <reflactable T, typename BufferSequence>
		static void to_binary(const T& value, BufferSequence& buff)
		{
			auto to_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ (to_binary(boost::pfr::get<I, T>(value), buff), ...); };

			to_binary_impl(std::make_index_sequence<boost::pfr::tuple_size_v<T>()>{});
		}

		template <integer_t T, typename BufferSequence>
		inline auto from_binary(const BufferSequence& buff) -> T
		{
			T value{};

			auto span_buff = std::span(buff);

			auto iter = std::find_if(span_buff.begin(), span_buff.end(), [](const auto s) { return s & 0x80 == 0; });

			auto length = std::distance(span_buff.begin(), iter);

			if (length == 0)
				return T{};

			auto& span_begin = span_buff[0];
			if (span_begin < 0x80)
				return static_cast<T>(span_begin);

			value += span_begin;
			value -= 0x80;

			auto sub_span = span_buff.subspan(1, length);

			int8_t temp_bit = 7;

			for (auto& s : sub_span)
			{
				if ((s & 0x80) != 0)
				{
					value += ((static_cast<T>(s) & 0x7f) << temp_bit);

					temp_bit += 7;
				}
				else
				{
					value += (static_cast<T>(s) << temp_bit);
				}
			}

			return value;
		}

		template <zig_zag T, typename BuffSequence>
		inline auto from_binary(const BuffSequence& buff) -> T
		{
			T value = from_binary<uint64_t>(buff);

			return static_cast<T>((value >> 1) ^ (~(value & 1) + 1));
		}

		template <typename T, typename BuffSequence>
		inline auto from_binary(const BuffSequence& buff) -> T
		{
			auto size = buff.size();

			constexpr auto t_size = sizeof(T);

			if (t_size > size) [[unlikely]]
				return {};

			auto sp = std::span(buff).subspan(0, t_size);

			return *static_cast<T*>(sp.data());
		}

		template <repeated_t T, typename BuffSequence>
		inline auto from_binary(const BuffSequence& buff) -> T
		{
			T value{};
			std::size_t size = from_binary<std::size_t>(buff);

			if (size == 0)
				return value;

			value.resize(size);

			for (int i = 0; i < size; ++i)
			{
				value[i] = from<typename T::value_type>(buff);
			}

			return value;
		}

		template <string_t T, typename BuffSequence>
		inline auto from_binary(const BuffSequence& buff) -> T
		{
			T value{};

			using value_type = typename BuffSequence::value_type;

			std::size_t size = from_binary<std::size_t>(buff);

			return std::string(std::span(buff.data(), size));
		}

		template <reflactable T, typename BuffSequence>
		inline auto from_binary(const BuffSequence& buff) -> T
		{
			auto from_binary_impl = [&]<std::size_t... I>(std::index_sequence<I...>)
			{ return T{ from_binary<boost::pfr::tuple_element_t<I, T>>(buff)... }; };

			return from_binary_impl(std::make_index_sequence<boost::pfr::tuple_size_v<T>()>{});
		}
	} // namespace serialize
} // namespace aquarius