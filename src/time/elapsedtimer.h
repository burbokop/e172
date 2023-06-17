// Copyright 2023 Borys Boiko

#pragma once

#include <cstdint>

namespace e172 {

class ElapsedTimer {
public:
    using Time = std::int64_t;

    ElapsedTimer(Time interval = 0);
    bool check(bool condition = true);
    Time elapsed() const;
    void reset();
    double progress() const;
    Time interval() const { return m_interval; }

private:
    Time m_interval;
    Time m_startPoint;
    Time m_checkPoint;
};

} // namespace e172
