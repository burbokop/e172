#include "differentiator.h"
#include "math.h"

e172::Differentiator::Differentiator(Differentiator::XMode xMode) {
    m_xMode = xMode;
}

double e172::Differentiator::proceed(double value) {
    double derivative = 0;
    if(hasLast && last != value) {
        if(m_xMode == Time) {
            const auto dt = c.deltaTime();
            if(dt != e172::Math::null) {
                derivative = (value - hasLast) / dt;
            }
        } else {
            derivative = value - hasLast;
        }
    }

    if(m_xMode == Time) {
        c.update();
    }

    last = value;
    hasLast = true;
    return derivative;
}

void e172::Differentiator::reset() {
    hasLast = false;
}

std::vector<double> e172::differentiate_vec(const std::pair<std::vector<double>, std::vector<double>> &cpair) {
    const auto count = std::min(cpair.first.size(), cpair.second.size());
    std::vector<double> result(count);

    double lastX;
    double lastY;
    bool hasLast = false;
    for(size_t i = 0; i < count; ++i) {
        if(hasLast) {
            const auto dx = cpair.first[i] - lastX;
            const auto dy = cpair.second[i] - lastY;
            if(dx != e172::Math::null) {
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
