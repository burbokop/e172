// Copyright 2023 Borys Boiko

#include "time.h"

#include <chrono>

namespace e172 {

Time::Value Time::currentMilliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
}

} // namespace e172
