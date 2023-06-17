// Copyright 2023 Borys Boiko

#pragma once

#include <ostream>

namespace e172 {

template<typename T, typename C = std::ostream::char_type>
concept Streamable = requires(std::basic_ostream<C> &os, const T &value) {
    {
        os << value
    } -> std::convertible_to<std::basic_ostream<C> &>;
};

} // namespace e172
