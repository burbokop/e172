// Copyright 2023 Borys Boiko

#pragma once

#include "../math/math.h"
#include "../todo.h"
#include <cstdint>
#include <limits>
#include <math.h>
#include <memory>

namespace e172 {

class Random
{
public:
    Random(std::uint64_t seed);
    Random(Random &&);
    ~Random();

    static Random uniq();

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
        } else if constexpr (std::is_same<T, float>::value) {
            return static_cast<float>(nextU16())
                   + (static_cast<float>(nextU16())
                      / static_cast<float>(std::numeric_limits<std::uint16_t>::max()));
        } else if constexpr (std::is_same<T, double>::value) {
            return static_cast<double>(nextU32())
                   + (static_cast<double>(nextU32())
                      / static_cast<double>(std::numeric_limits<std::uint32_t>::max()));
        } else if constexpr (std::is_same<T, long double>::value) {
            return static_cast<long double>(nextU64())
                   + (static_cast<long double>(nextU64())
                      / static_cast<long double>(std::numeric_limits<std::uint64_t>::max()));
        } else {
            todo();
        }
    }

    template<std::floating_point T>
    T nextNormalized()
    {
        if constexpr (std::is_same<T, float>::value) {
            return (static_cast<float>(nextU16())
                    / static_cast<float>(std::numeric_limits<std::uint16_t>::max()));
        } else if constexpr (std::is_same<T, double>::value) {
            return (static_cast<double>(nextU32())
                    / static_cast<double>(std::numeric_limits<std::uint32_t>::max()));
        } else if constexpr (std::is_same<T, long double>::value) {
            return (static_cast<long double>(nextU64())
                    / static_cast<long double>(std::numeric_limits<std::uint64_t>::max()));
        }
    }

    template<typename T>
    T nextInRange(T from, T to)
        requires std::is_arithmetic_v<T>
    {
        return from + Math::mod(next<T>(), (to - from));
    }

private:
    std::uint64_t nextU64();
    std::uint32_t nextU32();
    std::uint16_t nextU16();
    std::uint8_t nextU8();

    struct Impl;

private:
    static std::atomic<std::size_t> s_nextUniqSeedMultiplier;
    std::unique_ptr<Impl> m_impl;
};

} // namespace e172
