#ifndef REFLECTION_H
#define REFLECTION_H

#include <utility>

namespace e172 {

namespace reflection {

template <std::size_t i = 0>
struct ubiq { template<typename T> constexpr operator T&() const; };

template<typename T, std::size_t i0, std::size_t... i>
constexpr auto __detect_member_count(std::size_t &result, std::index_sequence<i0, i...>) -> decltype (T { ubiq<i0>(), ubiq<i>()... }) {
    result = sizeof... (i);
    return {};
}

template<typename T, std::size_t... i>
constexpr void __detect_member_count(std::size_t &result, std::index_sequence<i...>) {
    __detect_member_count<T>(result, std::make_index_sequence<sizeof... (i) - 1>());
}

template<typename T>
constexpr std::size_t member_count() {
    std::size_t result = 0;
    __detect_member_count<T>(result, std::make_index_sequence<sizeof (T)>());
    return result + 1;
}


}

}
#endif // REFLECTION_H
