#pragma once
#include <virgo/detail/basic_message.hpp>
#include <virgo/tcp/detail/layer_header.hpp>

namespace virgo
{
	namespace tcp
	{
		namespace detail
		{
			template <typename Header, typename Body, typename Allocator = std::allocator<Body>>
			class basic_message : public virgo::detail::basic_message<std::add_pointer_t<Body>, Allocator>, public layer_header
			{
			public:
				using header_type = Header;
				using base = virgo::detail::basic_message<std::add_pointer_t<Body>, Allocator>;
				using typename base::body_type;

			public:
				basic_message() = default;

				basic_message(const basic_message&) = default;

				basic_message(const layer_header& layer)
					: layer_header(layer)
				{}

				virtual ~basic_message() = default;

			public:
				std::ostream& operator<<(std::ostream& os) const
				{
					layer_header::operator<<(os);
					base::operator<<(os);
					header_ << os;
					return os;
				}

			public:
				header_type& header()
				{
					return header_;
				}

				const header_type& header() const
				{
					return header_;
				}

			public:
				template <typename BuffSequence>
				void commit(BuffSequence& buffer)
				{
					header_.commit(buffer);

					to_binary(this->body(), buffer);
				}

				template <typename BuffSequence>
				void consume(BuffSequence& buffer)
				{
					header_.consume(buffer);

					*this->get() = from_binary<Body>(buffer);
				}

			private:
				header_type header_;
			};
		} // namespace detail
	} // namespace tcp
} // namespace virgo

template <typename Header, typename Body>
inline std::ostream& operator<<(std::ostream& os, const virgo::tcp::detail::basic_message<Header, Body>& other)
{
	other << os;

	return os;
}