#pragma once
#include <aquarius_protocol/basic_http_header.hpp>
#include <aquarius_protocol/basic_message.hpp>
#include <aquarius_protocol/http_filed_type.hpp>
#include <aquarius_protocol/http_header_fileds.hpp>
#include <map>
#include <optional>

namespace aquarius
{
	template <bool Request, typename Body, typename Fields = http_header_fields,
			  typename Allocator = std::allocator<Body>>
	class basic_http_message : public basic_message<std::add_pointer_t<Body>, Allocator>,
							   public basic_http_header<Request>
	{
	public:
		using header_t = basic_http_header<Request>;

		using filed = Fields;

	public:
		basic_http_message() = default;

	public:
		filed& header()
		{
			return fields_;
		}

		const filed& header() const
		{
			return fields_;
		}

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			os << header() << this->body();

			return os;
		}

		template <typename BufferSequence>
		void commit(BufferSequence& buffer)
		{
			header_t::commit(buffer);

			fields_.commit(buffer);

			this->body().commit(buffer);
		}

		template <typename BufferSequence>
		void consume(BufferSequence& buffer)
		{
			header_t::consume(buffer);

			fields_.consume(buffer);

			this->body().consume(buffer);
		}

	private:
		filed fields_{};
	};

	template <bool Request, typename Body, typename Allocator>
	inline std::ostream& operator<<(std::ostream& os, const basic_http_message<Request, Body, Allocator>& other)
	{
		other << os;

		return os;
	}
} // namespace aquarius