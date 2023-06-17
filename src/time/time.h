// Copyright 2023 Borys Boiko

#pragma once

#include <cstdint>

namespace e172 {

class Time {
public:
    using Value = std::int64_t;

    static Value currentMilliseconds();
};

} // namespace e172
