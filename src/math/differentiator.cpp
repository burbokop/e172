// Copyright 2023 Borys Boiko

#include "differentiator.h"

#include "math.h"
#include <algorithm>

double e172::Differentiator::proceed(double value)
{
    double derivative = 0;
    if (m_hasLast && m_last != value) {
        if (m_xMode == Time) {
            const auto dt = m_c.deltaTime();
            if (dt != e172::Math::null) {
                derivative = (value - m_hasLast) / dt;
            }
        } else {
            derivative = value - m_hasLast;
        }
    }

    if (m_xMode == Time) {
        m_c.update();
    }

    m_last = value;
    m_hasLast = true;
    return derivative;
}

void e172::Differentiator::reset() {
    m_hasLast = false;
}

std::vector<double> e172::differentiateVec(
    const std::pair<std::vector<double>, std::vector<double>> &cpair)
{
    const auto count = std::min(cpair.first.size(), cpair.second.size());
    std::vector<double> result(count);

    double lastX;
    double lastY;
    bool hasLast = false;
    for (size_t i = 0; i < count; ++i) {
        if (hasLast) {
            const auto dx = cpair.first[i] - lastX;
            const auto dy = cpair.second[i] - lastY;
            if (dx != e172::Math::null) {
                result[i] = dy / dx;
            }
        } else {
            hasLast = true;
            result[i] = 0;
        }
        lastX = cpair.first[i];
        lastY = cpair.second[i];
    }
    return result;
}
