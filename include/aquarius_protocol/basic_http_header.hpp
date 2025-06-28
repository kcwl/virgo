#pragma once
#include <aquarius_protocol/basic_header.hpp>
#include <aquarius_protocol/http_method.hpp>
#include <aquarius_protocol/http_status.hpp>

namespace aquarius
{
	template<bool Request>
	class basic_http_header;

	template<>
	class basic_http_header<true> : public basic_header<true>
	{
    public:
        basic_http_header() = default;

    public:
        http_method method() const
        {
            return method_;
        }

        void method(http_method m)
        {
            method_ = m;
        }

    private:
        http_method method_;
	};

	template<>
	class basic_http_header<false> : public basic_header<false>
	{
    public:
        basic_http_header() = default;

    public:
        http_status result() const
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

        void reason(std::string_view r)
        {
            reason_ = r;
        }

    private:
        http_status status_;

        std::string_view reason_;
	};
}