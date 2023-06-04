#ifndef KDISCRETIZER_H
#define KDISCRETIZER_H

#include <vector>
#include <tuple>

namespace e172 {


template<typename Container>
class Discretizer {
    typedef typename Container::value_type val_type;
    std::size_t m_count;
    val_type lastX;
    bool frst = true;
public:
    Discretizer(std::size_t count = 8) { m_count = count; }
    inline Container proceed(const val_type &x) {
        if (frst) {
            frst = false;
            lastX = x;
            return { x };
        }

        typename Container::value_type result[m_count];
        auto dx = (x - lastX) / m_count;
        for (std::size_t i = 0; i < m_count; ++i) {
            result[i] = lastX + i * dx;
        }

        lastX = x;
        return Container(result, result + m_count);
    }
};

template<typename Container>
Container discretize(const Container& c, double count = 8) {
    Container result;
    if constexpr (std::is_same<Container, std::vector<typename Container::value_type>>::value) {
        result.reserve(c.size() * count);
    }
    Discretizer<Container> discretizer(count);
    for(auto it = c.begin(); it != c.end(); ++it) {
        const auto part = discretizer.proceed(*it);
        for(const auto& p : part) {
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

#endif // KDISCRETIZER_H
