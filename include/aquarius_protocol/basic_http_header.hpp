#pragma once
#include <aquarius_protocol/http_method.hpp>
#include <aquarius_protocol/http_status.hpp>
#include <aquarius_protocol/http_version.hpp>

namespace aquarius
{
	template <bool Request>
	class basic_http_header;

	template <>
	class basic_http_header<true>
	{
	public:
		basic_http_header() = default;

	public:
		http_method method() const
		{
			return method_;
		}

		http_method& method()
		{
			return method_;
		}

		void method(http_method m)
		{
			method_ = m;
		}

		http_version version() const
		{
			return version_;
		}

		http_version& version()
		{
			return version_;
		}

		void version(http_version v)
		{
			version_ = v;
		}

		const std::vector<std::string>& path() const
		{
			return path_;
		}

		std::vector<std::string>& path()
		{
			return path_;
		}

		void path(std::vector<std::string> p)
		{
			path_ = std::move(p);
		}

		std::vector<std::pair<std::string, std::string>>& querys()
		{
			return querys_;
		}

		const std::vector<std::pair<std::string, std::string>>& querys() const
		{
			return querys_;
		}

		void querys(std::vector<std::pair<std::string, std::string>> q)
		{
			querys_ = std::move(q);
		}

	private:
		http_method method_;

		http_version version_;

		std::vector<std::string> path_;

		std::vector<std::pair<std::string, std::string>> querys_;
	};

	template <>
	class basic_http_header<false>
	{
	public:
		basic_http_header() = default;

	public:
		http_status result() const
		{
			return status_;
		}

		http_status& result()
		{
			return status_;
		}

		void result(http_status s)
		{
			status_ = s;
		}

		std::string_view reason() const
		{
			return reason_;
		}

		std::string_view& reason()
		{
			return reason_;
		}

		void reason(std::string_view r)
		{
			reason_ = r;
		}

		http_version version() const
		{
			return version_;
		}

		http_version& version()
		{
			return version_;
		}

		void version(http_version v)
		{
			version_ = v;
		}

	private:
		http_status status_;

		std::string_view reason_;

		http_version version_;
	};
} // namespace aquarius