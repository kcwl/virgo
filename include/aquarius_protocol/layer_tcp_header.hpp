#pragma once
#include <iostream>
#include <cstdint>
#include <aquarius_protocol/binary.hpp>

namespace aquarius
{
	class layer_tcp_header
	{
		struct implement
		{
			int32_t version_;
			int32_t transfer_;
			uint64_t rpc_;
			uint64_t length_;
			uint32_t crc_;
			int64_t timestamp_;
		};

	public:
		constexpr static std::size_t impl_size = sizeof(implement);

	public:
		layer_tcp_header() = default;

		layer_tcp_header(const layer_tcp_header&) = default;

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			os << "version: " << impl_.version_ << " transfer: " << impl_.transfer_ << " rpc: " << impl_.rpc_
			   << " length: " << impl_.length_ << " crc: " << impl_.crc_ << " timestamp: " << impl_.timestamp_;
			return os;
		}

		template<typename BufferSequence>
		void mark(BufferSequence& buffer)
		{
			buffer.resize(impl_size);
		}

	public:
		int32_t version() const
		{
			return impl_.version_;
		}

		void version(int32_t version)
		{
			impl_.version_ = version;
		}

		int32_t transfer() const
		{
			return impl_.transfer_;
		}

		void transfer(int32_t transfer)
		{
			impl_.transfer_ = transfer;
		}

		uint64_t rpc() const
		{
			return impl_.rpc_;
		}

		void rpc(uint64_t rpc_id)
		{
			impl_.rpc_ = rpc_id;
		}

		uint64_t length() const
		{
			return impl_.length_;
		}

		void length(uint64_t len)
		{
			impl_.length_ = len;
		}
		uint32_t crc() const
		{
			return impl_.crc_;
		}

		void crc(uint32_t c)
		{
			impl_.crc_ = c;
		}

		int64_t timestamp() const
		{
			return impl_.timestamp_;
		}

		void timestamp(int64_t timestamp)
		{
			impl_.timestamp_ = timestamp;
		}

		template <typename BuffSequence>
		void commit(BuffSequence& buffer)
		{
			serialize::to_binary(impl_.version_, buffer);
			serialize::to_binary(impl_.transfer_, buffer);
			serialize::to_binary(impl_.rpc_, buffer);
			serialize::to_binary(impl_.length_, buffer);
			serialize::to_binary(impl_.crc_, buffer);
			serialize::to_binary(impl_.timestamp_, buffer);
		}

		template <typename BuffSequence>
		void consume(BuffSequence& buffer)
		{
			impl_.version_ = serialize::from_binary<int32_t>(buffer);
			impl_.transfer_ = serialize::from_binary<int32_t>(buffer);
			impl_.rpc_ = serialize::from_binary<uint64_t>(buffer);
			impl_.length_ = serialize::from_binary<uint64_t>(buffer);
			impl_.crc_ = serialize::from_binary<uint32_t>(buffer);
			impl_.timestamp_ = serialize::from_binary<int64_t>(buffer);
		}

	private:
		implement impl_;
	};
} // namespace aquarius
