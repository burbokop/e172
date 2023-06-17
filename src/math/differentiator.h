// Copyright 2023 Borys Boiko

#pragma once

#include <src/time/deltatimecalculator.h>
#include <tuple>
#include <utility>
#include <vector>

namespace e172 {

class Differentiator {
public:
    enum XMode { Time, Natural };

public:
    Differentiator(XMode xMode = Time)
        : m_xMode(xMode)
    {}

    double proceed(double value);
    void reset();

private:
    XMode m_xMode;
    e172::DeltaTimeCalculator<std::chrono::high_resolution_clock> m_c;
    double m_last;
    bool m_hasLast = false;
};

template<typename Container>
Container differentiate(const Container& c, Differentiator::XMode xMode = Differentiator::XMode::Time) {
    Container result;
    if constexpr (std::is_same<Container, std::vector<typename Container::value_type>>::value) {
        result.reserve(c.size());
    }
    Differentiator differentiator(xMode);
    for (auto it = c.begin(); it != c.end(); ++it) {
        result.push_back(differentiator.proceed(*it));
    }
    return result;
}

template<size_t i, typename Container>
Container differentiate(const Container& c, Differentiator::XMode xMode = Differentiator::XMode::Time) {
    Container result = c;
    std::get<i>(result) = differentiate(std::get<i>(c), xMode);
    return result;
}

std::vector<double> differentiateVec(
    const std::pair<std::vector<double>, std::vector<double>> &cpair);

} // namespace e172
