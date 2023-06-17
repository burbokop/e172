// Copyright 2023 Borys Boiko

#pragma once

#include <vector>
#include <tuple>

namespace e172 {

template<typename Container>
class Discretizer {
public:
    using Item = typename Container::value_type;
    Discretizer(std::size_t count = 8) { m_count = count; }
    Container proceed(const Item &x)
    {
        if (m_first) {
            m_first = false;
            m_lastX = x;
            return { x };
        }

        Container result;
        result.resize(m_count);
        const auto dx = (x - m_lastX) / m_count;
        for (std::size_t i = 0; i < m_count; ++i) {
            result[i] = m_lastX + i * dx;
        }

        m_lastX = x;
        return result;
    }

private:
    std::size_t m_count;
    Item m_lastX;
    bool m_first = true;
};

template<typename Container>
Container discretize(const Container& c, double count = 8) {
    Container result;
    if constexpr (std::is_same<Container, std::vector<typename Container::value_type>>::value) {
        result.reserve(c.size() * count);
    }
    Discretizer<Container> discretizer(count);
    for (auto it = c.begin(); it != c.end(); ++it) {
        const auto part = discretizer.proceed(*it);
        for (const auto &p : part) {
            result.push_back(p);
        }
    }
    return result;
}

template<std::size_t i0, std::size_t i1, typename Container>
void discretize(Container *c, double count = 8)
{
    std::get<i1>(*c) = discretize(std::get<i1>(*c), count);
    if constexpr(i0 < i1) {
        discretize<i0, i1 - 1>(c, count);
    }
}

template<std::size_t i0, std::size_t i1 = i0, typename Container>
Container discretize(Container c, double count = 8)
{
    discretize<i0, i1>(&c, count);
    return c;
}

} // namespace e172
