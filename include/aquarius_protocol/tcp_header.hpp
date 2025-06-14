#pragma once
#include <aquarius_protocol/detail/header_base.hpp>

namespace aquarius
{
	class tcp_request_header : public detail::header_base
	{
		using base_type = detail::header_base;

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
		bool pack(std::vector<char>& buff)
		{
			base_type::pack(buff);

			return true;
		}

		bool unpack(std::vector<char>& buff)
		{
			base_type::unpack(buff);

			return true;
		}
	};

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
		bool pack(std::vector<char>& buff)
		{
			base_type::pack(buff);

			serialize::to_binary(result_, buff);

			return true;
		}
		
		bool unpack(std::vector<char>& buff)
		{
			base_type::unpack(buff);

			result_ = serialize::from_binary<uint32_t>(buff);

			return true;
		}

	public:
		uint32_t result_;
	};
} // namespace aquarius

namespace std
{
	inline std::ostream& operator<<(std::ostream& os, const aquarius::tcp_request_header& other)
	{
		other << os;

		return os;
	}

	inline std::ostream& operator<<(std::ostream& os, const aquarius::tcp_response_header& other)
	{
		other << os;

		return os;
	}
} // namespace std