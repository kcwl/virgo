#pragma once
#include <reflection/reflect.hpp>
#include <reflection/member_bind_helper.hpp>

namespace aquarius
{
    template<typename _Ty>
    constexpr auto member_count();

    template<std::size_t I, typename _Ty>
    struct element
    {
        using type = std::remove_cvref_t<decltype(std::get<I>(member_bind_helper<_Ty, member_count<_Ty>()>::empty_values()))>;
    };

    template<std::size_t I, typename _Ty>
    using element_t = element<I, _Ty>::type;

    template<typename _Ty>
    constexpr auto name()
    {
        return reflect<_Ty>::topic();
    }

    template<std::size_t I, typename _Ty>
    constexpr auto field()
    {
        return std::get<I>(reflect<_Ty>::fields());
    }

    template<typename _Ty>
    constexpr auto member_count()
    {
        return reflect<_Ty>::fields().size();
    }

    template<std::size_t I, auto value>
    constexpr auto get()
    {
        using _Ty = std::remove_cvref_t<decltype(value)>;

        return std::get<I>(member_bind_helper<_Ty, member_count<_Ty>()>::template static_values<value>());
    }

    template<std::size_t I, typename _Ty>
    constexpr auto get(const _Ty& value) -> element_t<I, _Ty>
    {
        return std::get<I>(member_bind_helper<_Ty, member_count<_Ty>()>::values(value));
    }
}