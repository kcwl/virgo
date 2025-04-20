#pragma once
#include <detail/reflect.hpp>
#include <detail/member_bind_helper.hpp>

namespace aquarius
{
    template<typename T>
    constexpr auto member_count();

    template<std::size_t I, typename T>
    struct element
    {
        using type = std::remove_cvref_t<decltype(std::get<I>(member_bind_helper<T, member_count<T>()>::empty_values()))>;
    };

    template<std::size_t I, typename T>
    using element_t = element<I, T>::type;

    template<typename T>
    constexpr auto name()
    {
        return reflect<T>::topic();
    }

    template<std::size_t I, typename T>
    constexpr auto field()
    {
        return std::get<I>(reflect<T>::fields());
    }

    template<typename T>
    constexpr auto member_count()
    {
        return reflect<T>::fields().size();
    }

    template<std::size_t I, auto value>
    constexpr auto get()
    {
        using T = std::remove_cvref_t<decltype(value)>;

        return std::get<I>(member_bind_helper<T, member_count<T>()>::template static_values<value>());
    }

    template<std::size_t I, typename T>
    constexpr auto get(const T& value) -> element_t<I, T>
    {
        return std::get<I>(member_bind_helper<T, member_count<T>()>::values(value));
    }
}