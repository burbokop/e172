// Copyright 2023 Borys Boiko

#pragma once

#include <functional>

namespace e172 {

class Defer {
public:
    Defer(const std::function<void()> &f)
        : m_f(f)
    {}

    ~Defer()
    {
        if (m_f)
            m_f();
    }

private:
    std::function<void()> m_f;
};

} // namespace e172
