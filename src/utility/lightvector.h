// Copyright 2023 Borys Boiko

#pragma once

#include <tuple>
#include <utility>

namespace e172 {

template<typename X, typename Y>
struct LightVector {
    X x;
    Y y;

    operator std::pair<X, Y>() const { return {x, y}; }
    operator std::tuple<X, Y>() const { return {x, y}; }
};

} // namespace e172
