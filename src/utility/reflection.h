// Copyright 2023 Borys Boiko

#pragma once

#include <utility>

namespace e172::reflection {

template<std::size_t I = 0>
struct Ubiq
{
    template<typename T>
    constexpr operator T &() const;
};

template<typename T, std::size_t I0, std::size_t... I>
constexpr auto memberCount(std::size_t &result, std::index_sequence<I0, I...>)
    -> decltype(T{Ubiq<I0>(), Ubiq<I>()...})
{
    result = sizeof...(I);
    return {};
}

template<typename T, std::size_t... I>
constexpr void memberCount(std::size_t &result, std::index_sequence<I...>)
{
    memberCount<T>(result, std::make_index_sequence<sizeof...(I) - 1>());
}

template<typename T>
constexpr std::size_t memberCount()
{
    std::size_t result = 0;
    memberCount<T>(result, std::make_index_sequence<sizeof(T)>());
    return result + 1;
}

} // namespace e172::reflection
