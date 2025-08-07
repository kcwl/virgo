#pragma once
#include <aquarius_protocol/http_filed_type.hpp>
#include <map>

namespace aquarius
{

	class http_header_fields
	{
	public:
		template <typename T>
		T find(http_field_type f)
		{
			auto iter = fields_.find(f);

			if (iter == fields_.end())
				return {};

			return std::get<T>(iter->second);
		}

		template <typename T>
		void set_field(http_field_type f, T v)
		{
			auto iter = fields_.find(f);
			if (iter != fields_.end())
			{
				iter->second = v;
			}
			else
			{
				fields_[f] = v;
			}
		}

		void set_chunked(bool chunk)
		{
			fields_[http_field_type::chunked] = chunk ? "chunked" : std::string{};
		}

		bool chunked()
		{
			auto iter = fields_.find(http_field_type::chunked);
			if (iter == fields_.end())
				return false;

			return iter->second == "chunked" ? true : false;
		}

		bool has_content_length() const
		{
			return fields_.find(http_field_type::content_length) != fields_.end();
		}

		void content_length(std::optional<uint64_t> len)
		{
			if (!len.has_value())
				return;

			fields_[http_field_type::content_length] = std::to_string(*len);
		}

		std::optional<uint64_t> content_length()
		{
			auto iter = fields_.find(http_field_type::content_length);
			if (iter == fields_.end())
				return std::nullopt;

			return std::atoi(iter->second.c_str());
		}

		bool keep_alive()
		{
			auto iter = fields_.find(http_field_type::connection);
			if (iter == fields_.end())
				return false;

			return iter->second == "keep-alive" ? true : false;
		}

		void keep_alive(bool k)
		{
			fields_[http_field_type::connection] = k ? "keep-alive" : std::string{};
		}

	public:
		template <typename BufferSequence>
		void commit(BufferSequence& buffer)
		{}

		template <typename BufferSequence>
		void consume(BufferSequence& buffer)
		{}

	private:
		std::map<http_field_type, std::string> fields_;
	};

} // namespace aquarius