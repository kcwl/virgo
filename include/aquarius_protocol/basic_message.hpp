#pragma once
#include <boost/core/empty_value.hpp>
#include <aquarius_protocol/concepts.hpp>
#include <aquarius_protocol/binary.hpp>

namespace aquarius
{
	template <typename Header, typename Body, std::size_t Number>
	class basic_message : public Header, public boost::empty_value<Body>
	{
		using header_type = Header;

		using body_type = Body;

		using base_body_type = boost::empty_value<body_type>;

	public:
		inline constexpr static std::size_t proto = Number;

	public:
		basic_message()
			: header_type()
			, base_body_type()
		{}

		template <typename... Args>
		basic_message(Args&&... args)
			: header_type()
			, base_body_type(std::forward<Args>(args)...)
		{}

		basic_message(const basic_message&) = default;

		basic_message& operator=(const basic_message&) = default;

		basic_message(basic_message&& other) noexcept
			: basic_message()
		{
			*this = std::move(static_cast<header_type>(other));

			body_type{ other.get() }.swap(this->get());
		}

		basic_message& operator=(basic_message&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				*this = std::move(static_cast<header_type>(other));

				body_type{ other.get() }.swap(this->get());
			}

			return *this;
		}

		virtual ~basic_message() = default;

	public:
		bool operator==(const basic_message& other) const
		{
			return basic_message::proto == other.proto  && header() == other.header() && body() == other.body();
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			os << proto;

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

		template<typename Buffer>
		bool to_binary(Buffer& completed_buffer)
		{
			bool result = true;

			try
			{
				binary::to(completed_buffer, proto);

				header()->to_binary(completed_buffer);

				binary::to<body_type>(completed_buffer, this->get());
			}
			catch (...)
			{
				skip_error();

				result = false;
			}

			return result;
		}

		template<typename Buffer>
		bool from_binary(Buffer& completed_buffer)
		{
			bool result = true;

			try
			{
				std::size_t proto_number = binary::from<std::size_t>(completed_buffer);

				if (proto_number != proto)
					return false;

				header()->from_binary(completed_buffer);

				this->get() = binary::from<body_type>(completed_buffer);
			}
			catch (...)
			{
				skip_error();

				result = false;
			}

			return result;
		}

	protected:
		virtual void skip_error()
		{
			// archive{}.swap(this->completed_buffer_);
		}
	};
} // namespace aquarius