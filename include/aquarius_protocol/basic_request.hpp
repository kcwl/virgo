#pragma once
#include <aquarius_protocol/detail/basic_message.hpp>

namespace aquarius
{
	template <typename Protocol, typename Body>
	class basic_request : public detail::basic_message<Protocol, Body>
	{
		using base_type = detail::basic_message<Protocol, Body>;

	public:
		basic_request() = default;

	public:
		bool operator==(const basic_request& other) const
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