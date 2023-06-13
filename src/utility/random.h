#pragma once

#include "src/todo.h"
#include <cstdint>
#include <memory>

namespace e172 {

class Random
{
public:
    Random(std::uint64_t seed);
    ~Random();

    template<typename T>
    T next()
        requires std::is_arithmetic_v<T>
    {
        if constexpr (std::is_same<T, std::uint64_t>::value) {
            return nextU64();
        } else if constexpr (std::is_same<T, std::uint32_t>::value) {
            return nextU32();
        } else if constexpr (std::is_same<T, std::uint16_t>::value) {
            return nextU16();
        } else if constexpr (std::is_same<T, std::uint8_t>::value) {
            return nextU8();
        } else {
            todo;
        }
    }

    template<typename T>
    T nextInRange(T from, T to)
        requires std::is_arithmetic_v<T>
    {
        return from + (next<T>() % (to - from));
    }

private:
    std::uint64_t nextU64();
    std::uint32_t nextU32();
    std::uint16_t nextU16();
    std::uint8_t nextU8();

    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace e172
