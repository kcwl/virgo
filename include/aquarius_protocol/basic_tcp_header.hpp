#pragma once
#include <aquarius_protocol/basic_header.hpp>
#include <span>

namespace aquarius
{
	template <bool Request>
	class basic_tcp_header;

	template <>
	class basic_tcp_header<true> : public basic_header<true>
	{
	public:
		using base_type = basic_header<true>;

		using base_type::is_request;

	public:
		basic_tcp_header() = default;

		virtual ~basic_tcp_header() = default;

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			os << impl_.rpc_id << "\t" << impl_.crc << "\t" << impl_.length << "\t" << impl_.timestamp;

			return os;
		}

	public:
		uint64_t rpc_id() const
		{
			return impl_.rpc_id;
		}

		uint64_t length() const
		{
			return impl_.length;
		}

		uint32_t crc() const
		{
			return impl_.crc;
		}

		int64_t timestamp() const
		{
			return impl_.timestamp;
		}

		void length(uint64_t length)
		{
			impl_.length = length;
		}

		void crc(uint32_t crc)
		{
			impl_.crc = crc;
		}

		void timestamp(int64_t timestamp)
		{
			impl_.timestamp = timestamp;
		}

	public:
		template <typename BuffSequence>
		void unpack(BuffSequence& buffer)
		{
			base_type::unpack(buffer);

			impl_ = serialize::from_binary<impl>(buffer);
		}

		template <typename BufferSequence>
		void pack(BufferSequence& buffer)
		{
			base_type::pack(buffer);

			serialize::to_binary(impl_, buffer);
		}

	private:
		struct impl
		{
			uint64_t rpc_id;
			uint64_t length;
			uint32_t crc;
			int64_t timestamp;
		} impl_;
	};

	template <>
	class basic_tcp_header<false> : public basic_header<false>
	{
	public:
		using base_type = basic_header<false>;

		using base_type::is_request;

	public:
		basic_tcp_header()
			: impl_()
		{}

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			base_type::operator<<(os);

			os << impl_.crc << "\t" << impl_.length << "\t" << impl_.timestamp << "\t" << impl_.result;

			return os;
		}

	public:
		uint64_t length() const
		{
			return impl_.length;
		}

		uint32_t crc() const
		{
			return impl_.crc;
		}

		int64_t timestamp() const
		{
			return impl_.timestamp;
		}

		int32_t result() const
		{
			return impl_.result;
		}

		void length(uint64_t length)
		{
			impl_.length = length;
		}

		void crc(uint32_t crc)
		{
			impl_.crc = crc;
		}

		void timestamp(int64_t timestamp)
		{
			impl_.timestamp = timestamp;
		}

		void result(int32_t result)
		{
			impl_.result = result;
		}

	public:
		template <typename BufferSequence>
		void unpack(BufferSequence& buffer)
		{
			impl_ = serialize::from_binary<impl>(buffer);
		}

		template <typename BufferSequence>
		void pack(BufferSequence& buffer)
		{
			serialize::to_binary(impl_, buffer);
		}

	private:
		struct impl
		{
			uint64_t length;
			uint32_t crc;
			int64_t timestamp;
			int32_t result;
		} impl_;
	};
} // namespace aquarius
