#pragma once
#include <aquarius_protocol/binary.hpp>
#include <aquarius_protocol/concepts.hpp>
#include <boost/core/empty_value.hpp>

namespace aquarius
{
	namespace detail
	{
		template <typename Protocol, typename Body>
		class basic_message : public Protocol, public boost::empty_value<Body>
		{
			using header_type = typename Protocol::header;

			using body_type = Body;

			using base_body = boost::empty_value<body_type>;

		public:
			basic_message()
				: header_type()
				, base_body()
			{}

			basic_message(const basic_message&) = default;

			basic_message& operator=(const basic_message&) = default;

			basic_message(basic_message&& other) noexcept
				: header_type(std::move(other))
				, base_body(boost::empty_init, std::move(other.get()))
			{}

			basic_message& operator=(basic_message&& other) noexcept
			{
				if (this != std::addressof(other))
				{
					header_type::operator=(std::move(other));

					this->get() = std::move(other.get());
				}

				return *this;
			}

			virtual ~basic_message() = default;

		public:
			bool operator==(const basic_message& other) const
			{
				return basic_message::proto == other.proto && header() == other.header() && body() == other.body();
			}

			std::ostream& operator<<(std::ostream& os) const
			{
				os << header();

				os << body();

				return os;
			}

		public:
			const header_type& header() const
			{
				return *this;
			}

			header_type* header()
			{
				return this;
			}

			body_type& body()
			{
				return this->get();
			}

			const body_type& body() const
			{
				return this->get();
			}

			bool to_binary(const std::vector<char>& completed_buffer)
			{
				header_type::to_binary(completed_buffer);

				serialize::to_binary<body_type>(this->get(), completed_buffer);

				return true;
			}

			bool from_binary(const std::vector<char>& completed_buffer)
			{
				header_type::from_binary(completed_buffer);

				this->get() = serialize::from_binary<body_type>(completed_buffer);

				return true;
			}
		};
	} // namespace detail
} // namespace aquarius