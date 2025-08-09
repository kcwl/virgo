#pragma once
#include <aquarius_protocol/http_filed_type.hpp>
#include <map>
#include <ranges>
#include <boost/asio/buffer.hpp>

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

		void length(std::optional<uint64_t> len)
		{
			if (!len.has_value())
				return;

			fields_[http_field_type::content_length] = std::to_string(*len);
		}

		std::optional<uint64_t> length()
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
		bool consume(BufferSequence& buffer)
		{
			auto header_span = prase_header_span(buffer);

			constexpr auto delm="\r\n"sv;

			for (const auto line : std::views::split(header_span, delm))
			{
				if (line.empty())
					break;

				auto line_view = line | std::views::split(':');

				auto iter = line_view.begin();
				if (iter == line_view.end())
					return false;

				auto key = std::string_view(*iter);

				if (++iter == line_view.end())
					return false;

				fields_[from_string(key)] = std::string(std::string_view(*iter).data());
			}

			return true;
		}

	private:
		template <typename BufferSequence>
		std::span<char> prase_header_span(BufferSequence& buffer)
		{
			constexpr auto delm = "\r\n\r\n"sv;

			constexpr auto delm_size = delm.size();

			auto buf_view = std::views::slide(buffer, delm_size);

			auto iter = std::ranges::find_if(buf_view,
											[&](const auto& v)
											{
												if (v.size() < delm_size)
													return false;

												return std::string_view(v) == delm;
											});

			if(iter == buf_view.end())
				return {};

			auto pos = std::distance(buf_view.begin(),iter);

			auto result_span = buf_view.subspan(0, pos);

			auto dy_buffer = boost::asio::dynamic_buffer(buffer);

			dy_buffer.consume(result_span.size());

			return result_span;
		}

	private:
		std::map<http_field_type, std::string> fields_;
	};

} // namespace aquarius