#pragma once

#include <complex>
#include <functional>
#include <sstream>

#include "math.h"
#include <src/utility/buffer.h>
#include <src/utility/random.h>

namespace e172 {

constexpr double relativisticAdditionConstant = 0.7;

template<typename T>
    requires std::is_arithmetic_v<T>
class Vector
{
public:
    enum class Quarter : std::uint8_t {
        RightBottom = 0,
        LeftBottom = 1,
        LeftTop = 2,
        RightTop = 3,
        MaxValue = 4
    };

    constexpr Vector() = default;

    constexpr Vector(T x, T y)
        : m_x(x)
        , m_y(y)
    {}

    constexpr Vector(const Complex<T> &complex)
        : Vector(complex.real(), complex.imag())
    {}

    static Vector createByAngle(T module, T angle)
    {
        return Vector(module * Math::cos(angle), module * Math::sin(angle));
    }

    static Vector createRandom(Random &random, T max)
    {
        return Vector::createByAngle(random.nextInRange(static_cast<T>(0), max), random.next<T>());
    }

    static Vector createRandom(Random &&random, T max)
    {
        return Vector::createByAngle(random.nextInRange(static_cast<T>(0), max), random.next<T>());
    }

    constexpr Vector operator+(const Vector &term) const
    {
        return {m_x + term.m_x, m_y + term.m_y};
    }

    constexpr Vector operator+=(const Vector &term) { return {m_x += term.m_x, m_y += term.m_y}; }

    constexpr Vector operator-(const Vector &subtrahend) const
    {
        return {m_x - subtrahend.m_x, m_y - subtrahend.m_y};
    }

    constexpr Vector operator-=(const Vector &subtrahend)
    {
        return {m_x -= subtrahend.m_x, m_y -= subtrahend.m_y};
    }

    constexpr Vector operator*(T multiplier) const { return {m_x * multiplier, m_y * multiplier}; }

    constexpr friend Vector operator*(T scalar, const Vector &vector)
    {
        return {vector.m_x * scalar, vector.m_y * scalar};
    }

    constexpr Vector operator*=(T multiplier) { return {m_x *= multiplier, m_y *= multiplier}; }

    constexpr Vector operator/(T divider) const { return {m_x / divider, m_y / divider}; }

    constexpr Vector operator/=(T divider) { return {m_x /= divider, m_y /= divider}; }

    T operator*(const Vector &multiplier) const
    {
        return m_x * multiplier.m_x + m_y * multiplier.m_y;
    }

    constexpr bool sameDirection(const Vector &other) const { return *this * other > 0; }

    T incrementX(T term = 1) { return m_x += term; }
    T incrementY(T term = 1) { return m_y += term; }
    T decrementX(T term = 1) { return m_x -= term; }
    T decrementY(T term = 1) { return m_y -= term; }

    T module() const { return std::sqrt(m_x * m_x + m_y * m_y); }
    T cheapModule() const { return std::abs(m_x) + std::abs(m_y); }

    Vector normalized() const
    {
        const auto mod = module();
        if (!e172::Math::cmpf(mod, 0)) {
            return Vector(m_x / mod, m_y / mod);
        } else {
            return Vector();
        }
    }

    Vector projection(const Vector &b) const
    {
        const auto bm2 = b * b;
        if (bm2 != e172::Math::null) {
            const auto k = (*this * b) / bm2;
            return {k * b.x(), k * b.y()};
        }
        return {};
    }

    Vector leftNormal() const { return {m_y, -m_x}; }
    Vector rightNormal() const { return {-m_y, m_x}; }

    auto angle() const
    {
        const auto cos = normalized() * Vector(1, 0);
        if (m_y >= 0)
            return e172::Math::acos(cos);
        else
            return -e172::Math::acos(cos);
    }

    Vector relativisticAddition(Vector term, double c) const
    {
        double termModule = term.module();
        if (!e172::Math::cmpf(termModule, 0)) {
            c *= relativisticAdditionConstant;
            const Vector classicSum = *this + term;

            double thisModule = this->module();
            double classicSumModule = classicSum.module();

            double k = e172::Math::sqrt((1 + thisModule * termModule / (c * c)));
            double u = classicSumModule / k;

            return Vector(u * classicSum.m_x / classicSumModule,
                          u * classicSum.m_y / classicSumModule);
        }
        return *this;
    }

    T tg() const
    {
        if (!e172::Math::cmpf(m_y, 0))
            return -(m_x / m_y);
        return 0;
    }

    Quarter quarter(Quarter offset = Quarter::RightBottom) const
    {
        if (this->m_y >= 0) {
            if (this->m_x >= 0) {
                return Quarter::RightBottom + offset;
            } else {
                return Quarter::LeftBottom + offset;
            }
        } else {
            if (this->m_x >= 0) {
                return Quarter::RightTop + offset;
            } else {
                return Quarter::LeftTop + offset;
            }
        }
    }

    T map(Vector *destination, T value) const
    {
        T result = 0;
        T kd = destination->m_y - destination->m_x;
        T kt = m_y - m_x;

        if (e172::Math::cmpf(kt, 0))
            result = value * kd;
        else
            result = value * kd / kt;

        result += destination->m_x;

        return result;
    }

    T max() const { return std::max(m_x, m_y); }
    T min() const { return std::min(m_x, m_y); }

    Vector operator+(T term) const
    {
        const auto mod = module();
        if (mod != 0.0) {
            const auto mul = 1 + term / mod;
            return Vector(m_x * mul, m_y * mul);
        }
        return Vector();
    }

    Vector operator-(T subtrahend) const
    {
        const auto mod = module();
        if (mod != 0.0) {
            const auto mul = 1 - subtrahend / mod;
            return Vector(m_x * mul, m_y * mul);
        }
        return Vector();
    }

    Vector operator-() const { return {-m_x, -m_y}; }

    float float32X() const { return static_cast<float>(this->m_x); }
    float float32Y() const { return static_cast<float>(this->m_y); }
    auto intX() const { return static_cast<int>(this->m_x); }
    auto intY() const { return static_cast<int>(this->m_y); }
    auto size_tX() const { return static_cast<size_t>(this->m_x); }
    auto size_tY() const { return static_cast<size_t>(this->m_y); }

    T x() const { return this->m_x; }
    T y() const { return this->m_y; }

    T &x() { return this->m_x; }
    T &y() { return this->m_y; }

    Complex<T> toComplex() const { return {m_x, m_y}; }

    size_t fractalLevel(size_t limit = 256,
                        const ComplexFunction<T> &f = Math::sqr<Complex<T>>) const
    {
        return Math::fractalLevel(toComplex(), limit, f);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector &dt)
    {
        return os << "Vector(" << dt.x() << ", " << dt.y() << ")";
    }

    friend bool operator<(const Vector &vec0, const Vector &vec1)
    {
        return vec0.cheapModule() < vec1.cheapModule();
    }

    bool operator==(Vector vector) const
    {
        return e172::Math::cmpf(this->m_x, vector.m_x) && e172::Math::cmpf(this->m_y, vector.m_y);
    }

    bool operator!=(Vector vector) const
    {
        return !e172::Math::cmpf(this->m_x, vector.m_x) || !e172::Math::cmpf(this->m_y, vector.m_y);
    }

    static bool moduleLessComparator(const Vector &v0, const Vector &v1)
    {
        return v0.module() < v1.module();
    }

    void serialize(WriteBuffer &buf) const
    {
        buf.write(m_x);
        buf.write(m_y);
    }

    static std::optional<Vector> deserialize(ReadBuffer &buf)
    {
        Vector v;
        e172_chainingAssign(v.m_x, buf.read<T>());
        e172_chainingAssign(v.m_y, buf.read<T>());
        return v;
    }

    static std::optional<Vector> deserializeConsume(ReadBuffer &&buf) { return deserialize(buf); }

    template<typename R>
    Vector<R> into() const
    {
        return {static_cast<R>(m_x), static_cast<R>(m_y)};
    }

private:
    friend Quarter operator+(Quarter l, Quarter r)
    {
        using U = typename std::underlying_type<Quarter>::type;
        return Quarter((U(l) + U(r)) % U(Quarter::MaxValue));
    }

private:
    T m_x = 0;
    T m_y = 0;
};

} // namespace e172
