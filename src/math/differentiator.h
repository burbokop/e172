#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include <src/time/deltatimecalculator.h>
#include <vector>

namespace e172 {

class Differentiator {
    e172::DeltaTimeCalculator c;
    double last;
    bool hasLast = false;

    double derivative = 0;
public:
    enum {
        Time,
        Natural
    } typedef XMode;
private:
    XMode m_xMode;
public:
    Differentiator(XMode xMode = Time);
    double proceed(double value);
    void reset();
};


template<typename Container>
Container differentiate(const Container& c, Differentiator::XMode xMode = Differentiator::XMode::Time) {
    Container result;
    if constexpr (std::is_same<Container, std::vector<typename Container::value_type>>::value) {
        result.reserve(c.size());
    }
    Differentiator differentiator(xMode);
    for(auto it = c.begin(); it != c.end(); ++it) {
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




}

#endif // DIFFERENTIATOR_H
