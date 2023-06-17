// Copyright 2023 Borys Boiko

#pragma once

#include <cstddef>

namespace e172 {

class AverageCalculator
{
public:
    AverageCalculator() = default;
    double proceed(double value);
    double autoResetError() const { return m_autoResetError; }
    void setAutoResetError(double autoResetError) { m_autoResetError = autoResetError; }
    bool autoResetEnabled() const { return m_autoResetEnabled; }
    void setAutoResetEnabled(bool value) { m_autoResetEnabled = value; }

private:
    double m_average = 0;
    std::size_t m_count = 0;
    double m_autoResetError = 0;
    bool m_autoResetEnabled = false;
};

} // namespace e172
