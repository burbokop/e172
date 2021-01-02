#ifndef CONVERSION_H
#define CONVERSION_H

#include "variant.h"

namespace e172 {

template<typename A, typename B>
VariantMap toVariantMap(const std::pair<A, B> &value) {
    return VariantMap {
        { "first", value.first },
        { "second", value.second }
    };
}

template<typename Container>
VariantList toVariantList(const Container &container) {
    VariantList result;
    for(const auto& c : container) {
        result.push_back(Variant::fromValue(c));
    }
    return result;
}

template<typename Container>
VariantVector toVariantVector(const Container &container) {
    VariantVector result;
    result.reserve(container.size());
    for(const auto& c : container) {
        result.push_back(Variant::fromValue(c));
    }
    return result;
}

}

#endif // CONVERSION_H
