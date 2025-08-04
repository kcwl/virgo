#pragma once
#include <cstdint>
#include <iostream>

namespace aquarius
{

	class custom_tcp_request_header
	{
	public:
		uint64_t uuid() const
		{
			return uuid_;
		}
		void uuid(uint64_t id)
		{
			uuid_ = id;
		}

	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			os << uuid_;

			return os;
		}

		template <typename BuffSequence>
		void commit(BuffSequence& buffer)
		{
			serialize::to_binary(uuid_, buffer);
		}

		template <typename BuffSequence>
		void consume(BuffSequence& buffer)
		{
			uuid_ = serialize::from_binary<uint64_t>(buffer);
		}

	private:
		uint64_t uuid_;
	};

	class custom_tcp_response_header
	{
	public:
		std::ostream& operator<<(std::ostream& os) const
		{
			os << uuid_ << '\t' << result_;

			return os;
		}

	public:
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
			return result_;
		}
		void result(int32_t r)
		{
			result_ = r;
		}

		template <typename BuffSequence>
		void commit(BuffSequence& buffer)
		{
			serialize::to_binary(uuid_, buffer);

			serialize::to_binary(result_, buffer);
		}

		template <typename BuffSequence>
		void consume(BuffSequence& buffer)
		{
			uuid_ = serialize::from_binary<uint64_t>(buffer);

			result_ = serialize::from_binary<int32_t>(buffer);
		}

	private:
		uint64_t uuid_;
		int32_t result_;
	};
} // namespace aquarius