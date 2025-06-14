#pragma once
#include <aquarius_protocol/detail/basic_message.hpp>

namespace aquarius
{
	template <typename Protocl, typename Body>
	class basic_response : public detail::basic_message<Protocl, Body>
	{
		using base_type = detail::basic_message<Protocl, Body>;

	public:
		basic_response() = default;

	public:
		bool operator==(const basic_response& other) const
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