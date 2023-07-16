// Copyright 2023 Borys Boiko

#pragma once

#include <cmath>
#include <concepts>
#include <optional>

namespace e172 {

/**
 * @brief The Rational class - holds rational number
 */
template<std::integral T>
class Rational
{
public:
    static std::optional<Rational> make(T num, T den)
    {
        if (den != 0) {
            if constexpr (std::is_signed<T>::value) {
                return Rational(std::signbit(num * den) ? -std::abs(num) : std::abs(num),
                                std::abs(den));
            } else {
                return Rational(num, den);
            }
        } else {
            return std::nullopt;
        }
    }

    T num() const { return m_num; }
    T den() const { return m_den; }

    template<std::integral F>
    F into() const
    {
        return Rational(static_cast<F>(m_num), static_cast<F>(m_den));
    }

    template<typename F = T>
    F intoReal() const
        requires std::is_arithmetic<F>::value
    {
        return static_cast<F>(m_num) / static_cast<F>(m_den);
    }

    auto operator~() const { return inverted(); }

    template<typename F>
    friend F operator*(F rgs, Rational lhs)
        requires std::is_arithmetic<F>::value
    {
        return (rgs * lhs.m_num) / lhs.m_den;
    }

    std::optional<Rational> inverted() const
    {
        if (m_den != 0) {
            return Rational(m_den, m_num);
        } else {
            return std::nullopt;
        }
    }

private:
    Rational(T num, T den)
        : m_num(num)
        , m_den(den)
    {}

private:
    /// numerator
    T m_num;
    /// denominator
    T m_den;
};

} // namespace e172
