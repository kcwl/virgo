#pragma once
#include <aquarius_protocol/detail/basic_message.hpp>
#include <aquarius_protocol/detail/header_base.hpp>

namespace aquarius
{
	class tcp_response_header : public detail::header_base
	{
		using base_type = detail::header_base;

	public:
		tcp_response_header()
			: result_()
		{}

		tcp_response_header(const tcp_response_header& other)
			: base_type(other)
			, result_(other.result_)
		{}

		tcp_response_header(tcp_response_header&& other)
			: base_type(std::move(other))
			, result_(std::exchange(other.result_, 0))
		{}

		tcp_response_header& operator=(const tcp_response_header& other)
		{
			if (this != std::addressof(other))
			{
				base_type::operator=(other);

				result_ = other.result_;
			}

			return *this;
		}

		tcp_response_header& operator=(tcp_response_header&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				base_type::operator=(std::move(other));

				result_ = std::exchange(other.result_, 0);
			}

			return *this;
		}

	public:
		bool operator==(const tcp_response_header& other) const
		{
			return base_type::operator==(other) && result_ == other.result_;
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			os << result_;

			return os;
		}

		void set_result(uint32_t result)
		{
			result_ = result;
		}

	public:
		template <typename Buffer>
		void to_binary(Buffer& ar)
		{
			base_type::to_binary(ar);

			serialize::to_binary(ar, result_);
		}

		template <typename Buffer>
		void from_binary(Buffer& ar)
		{
			base_type::from_binary(ar);

			result_ = serialize::from_binary<uint32_t>(ar);
		}

	public:
		uint32_t result_;
	};

	template <typename Body, std::size_t Number>
	class tcp_response : public detail::basic_message<tcp_response_header, Body, Number>
	{
		using base_type = detail::basic_message<tcp_response_header, Body, Number>;

	public:
		static constexpr std::size_t proto = Number;

	public:
		tcp_response() = default;

	public:
		bool operator==(const tcp_response& other) const
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
	inline std::ostream& operator<<(std::ostream& os, const aquarius::tcp_response_header& other)
	{
		other << os;

		return os;
	}
} // namespace std