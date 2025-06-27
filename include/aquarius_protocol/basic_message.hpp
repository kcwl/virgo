#pragma once
#include <aquarius_protocol/binary.hpp>
#include <aquarius_protocol/concepts.hpp>
#include <boost/core/empty_value.hpp>

namespace aquarius
{
	template <typename Body>
	class basic_message : public boost::empty_value<Body>
	{
	public:
		using body_type = Body;

		using base_body = boost::empty_value<body_type>;

		using base_type = basic_message;

	public:
		basic_message() = default;

		basic_message(const basic_message&) = default;

		basic_message& operator=(const basic_message&) = default;

		basic_message(basic_message&& other) noexcept
			: base_body(boost::empty_init, std::move(other.get()))
		{}

		basic_message& operator=(basic_message&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				this->get() = std::move(other.get());
			}

			return *this;
		}

		virtual ~basic_message() = default;

	public:
		bool operator==(const basic_message& other) const
		{
			return body() == other.body();
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			os << body();

			return os;
		}

	public:
		body_type& body()
		{
			return this->get();
		}

		const body_type& body() const
		{
			return this->get();
		}
	};
} // namespace aquarius