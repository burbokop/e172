#ifndef INTERGRATOR_H
#define INTERGRATOR_H

#include <vector>

namespace e172 {


class Intergrator {
    double m_alpha;

    struct ChannelState {
        double lastY;
        bool frst = true;
    };

    std::vector<ChannelState> channelStateArray;
public:
    Intergrator(double alpha = 0.9);


    double proceed(double value, size_t channel = 0);

};

template<typename OriginalIt>
class IntergratorIterator {
    OriginalIt m_begin, m_end;
    Intergrator m_intergrator;
    IntergratorIterator(OriginalIt begin, OriginalIt end, double alpha = 0.9) {
        m_begin = begin;
        m_end = end;
        m_intergrator = alpha;
    }
    auto operator++() {
        return m_intergrator.proceed(m_begin++, 0);
    }
};

template<typename Container>
Container intergate(const Container& c, double alpha = 0.9) {
    Container result;
    if constexpr (std::is_same<Container, std::vector<typename Container::value_type>>::value) {
        result.reserve(c.size());
    }
    Intergrator intergrator(alpha);
    for(auto it = c.begin(); it != c.end(); ++it) {
        result.push_back(intergrator.proceed(*it, 0));
    }
    return result;
}

template<size_t i, typename Container>
Container intergate(const Container& c, double alpha = 0.9) {
    Container result = c;
    std::get<i>(result) = intergate(std::get<i>(c), alpha);
    return result;
}


}

#endif // INTERGRATOR_H
