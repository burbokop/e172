#ifndef KDISCRETIZER_H
#define KDISCRETIZER_H

#include <vector>

namespace e172 {


template<typename Container>
class Discretizer {
    typedef typename Container::value_type val_type;
    size_t m_count;
    val_type lastX;
    bool frst = true;
public:
    Discretizer(size_t count = 8) { m_count = count; }
    inline Container proceed(const val_type &x) {
        if (frst) {
            frst = false;
            lastX = x;
            return { x };
        }

        typename Container::value_type result[m_count];
        auto dx = (x - lastX) / m_count;
        for (size_t i = 0; i < m_count; ++i) {
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

}

#endif // KDISCRETIZER_H
