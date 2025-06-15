#pragma once
#include <aquarius_protocol/detail/basic_message.hpp>

namespace aquarius
{
	template <typename Protocol, typename Body>
	class basic_request : public detail::basic_message<Body>
	{
		using base_type = detail::basic_message<Body>;

		using header_type = typename Protocol::request_header;

	public:
		basic_request() = default;

	public:
		bool operator==(const basic_request& other) const
		{
			return base_type::operator==(other) && header() == other.header();
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			header_ << os;

			base_type::operator<<(os);

			return os;
		}

	public:
		const header_type& header() const
		{
			return header_;
		}

		header_type* header()
		{
			return &header_;
		}

		virtual std::vector<char> pack()
		{
			std::vector<char> completed_buffer{};

			if (!header_.pack(completed_buffer))
				return {};

			if (!base_type::pack(completed_buffer))
				return {};

			return completed_buffer;
		}

		virtual bool unpack(std::vector<char>& completed_buffer)
		{
			if (!header_.unpack(completed_buffer))
				return false;

			if (!base_type::unpack(completed_buffer))
				return false;

			return true;
		}

	private:
		header_type header_;
	};
} // namespace aquarius