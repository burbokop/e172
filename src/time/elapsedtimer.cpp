// Copyright 2023 Borys Boiko

#include "elapsedtimer.h"

#include "time.h"

namespace e172 {

ElapsedTimer::ElapsedTimer(Time interval)
{
    m_interval = interval;
    reset();
}

bool ElapsedTimer::check(bool condition) {
    if (m_interval > 0) {
        const auto milliseconds = e172::Time::currentMilliseconds();
        if (milliseconds - m_checkPoint > m_interval && condition) {
            m_checkPoint = milliseconds;
            return true;
        }
        return false;
    }
    return true;
}

ElapsedTimer::Time ElapsedTimer::elapsed() const
{
    return e172::Time::currentMilliseconds() - m_startPoint;
}

void ElapsedTimer::reset() {
    m_startPoint = e172::Time::currentMilliseconds();
    m_checkPoint = m_startPoint;
}

double ElapsedTimer::progress() const {
    const double milliseconds = static_cast<double>(e172::Time::currentMilliseconds());
    if (m_interval == 0)
        return 0;
    return (milliseconds - m_checkPoint) / m_interval;
}

} // namespace e172
