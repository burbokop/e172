// Copyright 2023 Borys Boiko

#pragma once

#include <optional>
#include <type_traits>

namespace e172 {

template<typename T, typename Enable = void>
struct IsOptional : std::false_type
{};

template<typename T>
struct IsOptional<std::optional<T>> : std::true_type
{};

template<class... T>
struct Overloaded : T...
{
    using T::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

} // namespace e172
