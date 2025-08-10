#pragma once

namespace aquarius
{
	enum class http_field_type
	{
		error,
		connection,
		content_length,
		chunked
	};

	inline http_field_type from_string(std::string_view str)
	{
		if (str == "Accept")
			return http_field_type::connection;
		else if (str == "Content-Length")
			return http_field_type::content_length;

		return http_field_type::error;
	}
} // namespace aquarius