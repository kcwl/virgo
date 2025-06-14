#pragma once
#include <aquarius_protocol/detail/basic_message.hpp>
#include <aquarius_protocol/detail/header_base.hpp>

namespace aquarius
{
	template<typename Raw>
	class tcp_request_header : public detail::header_base<Raw>
	{
		using base_type = detail::header_base<Raw>;

	public:
		tcp_request_header() = default;

		tcp_request_header(const tcp_request_header& other)
			: base_type(other)
		{}

		tcp_request_header(tcp_request_header&& other) noexcept
			: base_type(std::move(other))
		{}

		tcp_request_header& operator=(const tcp_request_header& other)
		{
			if (this != std::addressof(other))
			{
				base_type::operator=(other);
			}

			return *this;
		}

		tcp_request_header& operator=(tcp_request_header&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				base_type::operator=(std::move(other));

				return *this;
			}
		}

	public:
		bool operator==(const tcp_request_header& other) const
		{
			return base_type::operator==(other);
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			return os;
		}

	public:
		void to_binary(std::vector<char>& buff)
		{
			base_type::to_binary(buff);
		}

		void from_binary(const std::vector<char>& buff)
		{
			base_type::from_binary(buff);
		}
	};

	template <typename Raw, typename Body, std::size_t Number>
	class tcp_request : public detail::basic_message<tcp_request_header<Raw>, Body, Number>
	{
		using base_type = detail::basic_message<tcp_request_header<Raw>, Body, Number>;

	public:
		static constexpr std::size_t proto = Number;

	public:
		tcp_request() = default;

	public:
		bool operator==(const tcp_request& other) const
		{
			return base_type::operator==(other);
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			return os;
		}
	};
} // namespace aquarius

namespace std
{
	template<typename Raw>
	inline std::ostream& operator<<(std::ostream& os, const aquarius::tcp_request_header<Raw>& other)
	{
		other << os;

		return os;
	}
} // namespace std