#ifndef LIGHTVECTOR_H
#define LIGHTVECTOR_H

#include <tuple>

namespace e172 {

template<typename X, typename Y>
struct LightVector {
    X x;
    Y y;
    inline operator std::pair<X, Y>() const { return { x, y }; }
    inline operator std::tuple<X, Y>() const { return { x, y }; }
};

}
#endif // LIGHTVECTOR_H
