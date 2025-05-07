#pragma once
#include <boost/core/empty_value.hpp>
#include <aquarius_protocol/concepts.hpp>
#include <aquarius_protocol/header.hpp>

namespace aquarius
{
	template <standard_header_t Header, standard_layout_t Body, std::size_t Number>
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
			header_type{ other }.swap(*this);

			body_type{ other.get() }.swap(this->get());
		}

		basic_message& operator=(basic_message&& other) noexcept
		{
			if (this != std::addressof(other))
			{
				header_type{ other }.swap(*this);

				body_type{ other.get() }.swap(this->get());
			}

			return *this;
		}

		virtual ~basic_message() = default;

	public:
		bool operator==(const basic_message& other) const
		{
			return basic_message::proto == other.proto  && *header() == *other.header() && body() == other.body();
		}

		std::ostream& operator<<(std::ostream& os) const
		{
			os << proto;

			os << *header();

			os << body();

			return os;
		}

	public:
		const header_type* header() const
		{
			return this;
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

		auto buffer() -> archive&&
		{
			return std::move(completed_buffer_);
		}

		std::size_t collect(archive& ar)
		{
			return this->completed_buffer_.sputn((archive::char_type*)ar.data().data(), ar.size());
		}

		bool complete()
		{
			return this->from_binary();
		}

		bool disperse(archive& ar, std::size_t unit_size)
		{
			if (!this->to_binary())
				return false;

			std::size_t buffer_size = this->completed_buffer_.size();

			while (buffer_size != 0)
			{
				auto disperse_size = std::min<std::size_t>(unit_size, buffer_size);

				ar.sputn((archive::char_type*)this->completed_buffer_.data().data(), disperse_size);

				buffer_size -= disperse_size;
			}

			return true;
		}

	private:
		bool to_binary()
		{
			bool result = true;

			try
			{
				binary::to(completed_buffer_, proto);

				header()->to_binary(completed_buffer_);

				binary::to<body_type>(completed_buffer_, this->get());
			}
			catch (...)
			{
				skip_error();

				result = false;
			}

			return result;
		}

		bool from_binary()
		{
			bool result = true;

			try
			{
				std::size_t proto_number = binary::from<std::size_t>(completed_buffer_);

				if (proto_number != proto)
					return false;

				header()->from_binary(completed_buffer_);

				this->get() = binary::from<body_type>(completed_buffer_);
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

	private:
		archive completed_buffer_;
	};
} // namespace aquarius