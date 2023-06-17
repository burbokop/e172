// Copyright 2023 Borys Boiko

#pragma once

#include <chrono>

namespace e172 {

template<typename T>
    requires std::chrono::is_clock_v<T>
class DeltaTimeCalculator
{
    using Clock = T;
    using TimePoint = typename Clock::time_point;

public:
    DeltaTimeCalculator() = default;

    void update() { m_deltaTime = calculateDuration() * 0.000001 * m_timeSpeed; }

    /**
     * @brief deltaTime
     * @return seconds
     */
    double deltaTime() { return m_deltaTime; }

    void setTimeSpeed(double value) { m_timeSpeed = value; }

private:
    std::int64_t calculateDuration()
    {
        const auto now = Clock::now();
        std::int64_t microseconds
            = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastTimePoint).count();
        m_lastTimePoint = now;
        return microseconds;
    }

private:
    TimePoint m_lastTimePoint = Clock::now();
    double m_deltaTime = 0;
    double m_timeSpeed = 1;
};

} // namespace e172
