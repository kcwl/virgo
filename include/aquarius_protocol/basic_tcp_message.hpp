#pragma once
#include <aquarius_protocol/basic_message.hpp>
#include <aquarius_protocol/layer_tcp_header.hpp>

namespace aquarius
{
	template <typename Header, typename Body, typename Allocator = std::allocator<Body>>
	class basic_tcp_message : public basic_message<std::add_pointer_t<Body>, Allocator>, public layer_tcp_header
	{
	public:
		using header_type = Header;
		using base = basic_message<std::add_pointer_t<Body>, Allocator>;
		using typename base::body_type;
		using layer_header = layer_tcp_header;

	public:
		basic_tcp_message() = default;

		basic_tcp_message(const basic_tcp_message&) = default;

		basic_tcp_message(const layer_tcp_header& layer)
			: layer_tcp_header(layer)
		{

		}

		virtual ~basic_tcp_message() = default;

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			layer_tcp_header::operator<<(os);
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

			base::commit(buffer);
		}

		template <typename BuffSequence>
		void consume(BuffSequence& buffer)
		{
			header_.consume(buffer);

			base::consume(buffer);
		}

	private:
		header_type header_;
	};
} // namespace aquarius

template <typename Header, typename Body>
inline std::ostream& operator<<(std::ostream& os, const aquarius::basic_tcp_message<Header, Body>& other)
{
	other << os;

	return os;
}