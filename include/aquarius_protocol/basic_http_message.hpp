#pragma once
#include <aquarius_protocol/basic_message.hpp>
#include <aquarius_protocol/basic_http_header.hpp>
#include <aquarius_protocol/http_filed_type.hpp>
#include <optional>
#include <map>
#include <variant>

namespace aquarius
{
    template<bool Request, typename Body, typename Allocator = std::allocator<Body>>
    class basic_http_message : public basic_http_header<Request>, public basic_message<Body*, Allocator>
    {
    public:
        using header_type = basic_http_message;

    public:
        basic_http_message() = default;

    public:
        header_type* header()
        {
            return this;
        }

        const header_type* header() const
        {
            return this;
        }

    public:
        std::ostream& operator<<(std::ostream& os) const
        {
            os << header() << this->body();

            return os;
        }

    public:
        template<typename T>
        T find(http_field_type f)
        {
            auto iter = fields_.find(f);

            if (iter == fields_.end())
                return {};

            return std::get<T>(iter->second);
        }

        template<typename T>
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

    protected:
        void set_chunked(bool chunk)
        {
            fields_[http_field_type::chunked] = chunk;
        }

        bool chunked()
        {
            return std::get<std::string_view>(fields_[http_field_type::chunked]) == "chunked" ? true : false;
        }

        bool has_content_length() const
        {
            return fields_.find(http_field_type::content_length) != fields_.end();
        }

        void content_length(std::optional<uint64_t> len)
        {
            if (!len.has_value())
                return;

            fields_[http_field_type::content_length] = *len;
        }

        std::optional<uint64_t> content_length()
        {
            auto iter = fields_.find(http_field_type::content_length);
            if (iter == fields_.end())
                return std::nullopt;

            return std::get<uint64_t>(fields_[http_field_type::content_length]);
        }

        bool keep_alive()
        {
            auto iter = fields_.find(http_field_type::connection);
            if (iter == fields_.end())
                return true;

            return std::get<bool>(iter->second);
        }

        void keep_alive(bool k)
        {
            auto iter = fields_.find(http_field_type::connection);
            if (iter != fields_.end())
            {
                if (std::get<bool>(iter->second) == k)
                    return;
            }

            fields_[http_field_type::connection] = k;
        }

    private:
        std::map<http_field_type, std::variant<bool, uint64_t, std::string_view>> fields_;
    };

    template<bool Request, typename Body, typename Allocator>
    inline std::ostream& operator<<(std::ostream& os, const basic_http_message<Request, Body, Allocator>& other)
    {
        other << os;

        return os;
    }
}