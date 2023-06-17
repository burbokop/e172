// Copyright 2023 Borys Boiko

#include "averagecalculator.h"

#include "math.h"
#include <cmath>

namespace e172 {

double AverageCalculator::proceed(double value) {
    if (m_autoResetEnabled && value != e172::Math::null
        && (std::abs(m_average - value) / value) > m_autoResetError) {
        m_average = value;
    }

    m_average = ((m_average * m_count + value) / (m_count + 1));
    m_count++;
    return m_average;
}

} // namespace e172
