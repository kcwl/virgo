#pragma once
#include <boost/core/empty_value.hpp>
#include <message/header.hpp>

namespace aquarius
{

    template<typename T>
    using reference_t = T&;

    template<typename T>
    concept standard_layout_t = requires (T value)
    {
        std::is_standard_layout_v<std::remove_cvref_t<T>>&& std::is_trivial_v<std::remove_cvref_t<T>>;

        value.swap(std::declval<reference_t<T>>());
    };

    template<typename T>
    concept standard_header_t = requires(T value)
    {
        value.template to_binary<archive_t<class Archive>>(std::declval<reference_t<Archive>>());

        //value.template from_binary<archive_t<class Archive>>(std::declval<reference_t<Archive>>());
    };


    inline constexpr std::size_t rpc_module = 0;

    template<archive_t Archive, standard_header_t Header, standard_layout_t Body, std::size_t Type, std::size_t Number>
    class basic_message : public Header, public boost::empty_value<Body>
    {
        using header_type = Header;
        using body_type = Body;
        using base_body_type = boost::empty_value<body_type>;

        inline constexpr static std::size_t poll_type = Type;

        inline constexpr static std::size_t proto = Number;

    public:
        using archive_type = Archive;

    public:
        basic_message()
            : header_type()
            , base_body_type()
        {

        }

        template<typename... Args>
        basic_message(Args&&... args)
            : header_type()
            , base_body_type(std::forward<Args>(args)...)
        {

        }

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
        header_type* header()
        {
            return this;
        }

        body_type& body()
        {
            return this->get();
        }

        void to_binary(archive_type& ar)
        {
            try
            {
                 header().template to_binary(ar);

                 binary<archive_type>::template to<body_type>(ar, this->get());
            }
            catch (...)
            {
                skip_error(ar);
            }
        }

        void from_binary(archive_type& ar)
        {
            try
            {
                 header().template from_binary(ar);

                 this->get() = binary<archive_type>::template from<body_type>(ar);
            }
            catch (...)
            {
                skip_error(ar);
            }
        }

    protected:
        virtual void skip_error(archive_type& ar) = 0;


    protected:
        archive_type completed_buffer_;
    };

    template<archive_t Archive, standard_layout_t Body, std::size_t Type, std::size_t Number>
    class basic_request : public basic_message<Archive, basic_request_header, Body, Type, Number>
    {
        using base_type = basic_message<Archive, basic_request_header, Body, Type, Number>;

        using typename base_type::archive_type;

    public:
        basic_request() = default;

    public:
        void collect(archive_type& ar)
        {
            this->completed_buffer_.sputn(ar.data(), ar.size());
        }

        void complete()
        {
            this->from_binary(this->completed_buffer_);
        }

    protected:
        virtual void skip_error(archive_type& ar) override
        {
            if (this->header()->total_length == 0)
                return;

            ar.consume(std::min<int32_t>(this->header()->total_length, static_cast<int>(ar.size())));
        }
    };

    template<archive_t Archive, standard_layout_t Body, std::size_t Type, std::size_t Number>
    class basic_response : public basic_message<Archive, basic_response_header, Body, Type, Number>
    {
        using base_type = basic_message<Archive, basic_request_header, Body, Type, Number>;

        using typename base_type::archive_type;

    public:
        basic_response() = default;

    public:
        void disperse(std::vector<archive_type>& ar, std::size_t unit_size)
        {
            this->to_binary(this->completed_buffer_);

            std::size_t buffer_size = this->completed_buffer_.size();

            while (buffer_size != 0)
            {
                ar.emplace_back();
                auto& back = ar.back();

                auto disperse_size = std::min<std::size_t>(unit_size, buffer_size);

                back.sputn(this->completed_buffer_.data(), disperse_size);

                buffer_size -= disperse_size;
            }
        }

    protected:
        virtual void skip_error(archive_type& ar) override
        {
            if (this->header().total_length == 0)
                return;

            ar.cosume(std::min<int32_t>(this->header().total_length, ar.size()));
        }
    };

}