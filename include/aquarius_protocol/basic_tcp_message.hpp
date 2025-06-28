#pragma once
#include <aquarius_protocol/basic_message.hpp>
#include <aquarius_protocol/basic_tcp_header.hpp>

namespace aquarius
{
	template <bool Request, typename Body, typename Allocator = std::allocator<Body>>
	class basic_tcp_message : public basic_message<Body*, Allocator>, public basic_tcp_header<Request>
	{
	public:
		using header_type = basic_tcp_header<Request>;

		using base_type = basic_message<Body*, Allocator>;

		using typename base_type::body_type;

		using self_type = basic_tcp_message;

	public:
		basic_tcp_message() = default;

		virtual~basic_tcp_message() = default;

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			header_type::operator<<(os);

			os << uuid_ << "\t" << result_ << "\t";

			base_type::operator<<(os);

			return os;
		}

	public:
		self_type* header()
		{
			return this;
		}

		const self_type* header() const
		{
			return this;
		}

		uint64_t uuid() const
		{
			return uuid_;
		}

		void uuid(uint64_t id)
		{
			uuid_ = id;
		}

		int32_t result() const
		{
			static_assert(!Request, "response has result function");

			return result_;
		}

		void result(int32_t r)
		{
			static_assert(!Request, "response has result function");

			result_ = r;
		}
		
		template<typename BuffSequence>
		void pack(BuffSequence& completed_buffer)
		{
			header_type::pack(completed_buffer);

			serialize::to_binary<uint64_t>(uuid_, completed_buffer);

			if constexpr (!Request)
			{
				serialize::to_binary<int32_t>(result_, completed_buffer);
			}

			serialize::to_binary<body_type>(*this->get(), completed_buffer);
		}

		template<typename BuffSequence>
		void unpack(BuffSequence& completed_buffer)
		{
			header_type::unpack(completed_buffer);

			uuid_ = serialize::from_binary<uint64_t>(completed_buffer);

			if constexpr (!Request)
			{
				result_ = serialize::from_binary<int32_t>(completed_buffer);
			}

			*this->get() = serialize::from_binary<body_type>(completed_buffer);
		}

	private:
		uint64_t uuid_;

		int32_t result_;
	};
} // namespace aquarius

template<bool Request, typename Body>
inline std::ostream& operator<<(std::ostream& os, const aquarius::basic_tcp_message<Request, Body>& other)
{
	other << os;

	return os;
}