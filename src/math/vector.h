#ifndef VECTOR_H
#define VECTOR_H

#define RELATIVISTIC_ADDITION_CONSTANT 0.7

#include <complex>
#include <functional>
#include <sstream>

#include <src/utility/buffer.h>

namespace e172 {

typedef std::complex<double> Complex;
typedef std::function<Complex(const Complex&)> ComplexFunction;

class Vector {
public:
    enum Quarter { QUARTER_RIGHT_DOWN = 0, QUARTER_LEFT_DOWN, QUARTER_LEFT_UP, QUARTER_RIGHT_UP };

    constexpr Vector() = default;
    constexpr Vector(double x, double y)
        : m_x(x)
        , m_y(y)
    {}

    Vector(const Complex &complex);
    static Vector createByAngle(double module, double angle); //checkpoint
    static Vector createRandom(double max);

    Vector operator+(const Vector &term) const;
    void operator+=(const Vector &term);
    Vector operator-(const Vector &subtrahend) const;
    void operator-=(const Vector &subtrahend);

    Vector operator*(double multiplier) const;
    friend Vector operator*(double scalar, const Vector& vector);
    void operator*=(double multiplier);
    Vector operator/(double divider) const;
    void operator/=(double divider);


    bool operator==(Vector vector) const;
    bool operator!=(Vector vector) const;

    double operator*(const Vector& multiplier) const;

    bool sameDirection(const Vector& other) const;

    void incrementX(double term = 1) { m_x += term; }
    void incrementY(double term = 1) { m_y += term; }
    void decrementX(double term = 1) { m_x -= term; }
    void decrementY(double term = 1) { m_y -= term; }

    double module() const; //checkpoint
    double cheapModule() const;
    Vector normalized() const;

    Vector projection(const Vector &b) const;
    Vector leftNormal() const;
    Vector rightNormal() const;

    double angle() const; //checkpoint

    Vector relativisticAddition(Vector term, double c) const;
    double tg() const;
    unsigned quarter(unsigned offset = 0) const;
    double map(Vector *destination, double value) const;
    inline double max() const { return std::max(m_x, m_y); }
    inline double min() const { return std::min(m_x, m_y); }

    Vector operator+(double term) const;
    Vector operator-(double subtrahend) const;

    Vector operator-() const;

    inline float float32X() const { return static_cast<float>(this->m_x); }
    inline float float32Y() const { return static_cast<float>(this->m_y); }
    inline auto intX() const { return static_cast<int>(this->m_x); }
    inline auto intY() const { return static_cast<int>(this->m_y); }
    inline auto size_tX() const { return static_cast<size_t>(this->m_x); }
    inline auto size_tY() const { return static_cast<size_t>(this->m_y); }
    inline double x() const { return this->m_x; }
    inline double y() const { return this->m_y; }

    double &x() { return this->m_x; }
    double &y() { return this->m_y; }

    inline Complex toComplex() const { return { m_x, m_y }; }

    size_t fractalLevel(size_t limit = 256, const ComplexFunction& f = [](const Complex &x){ return x * x; }) const;

    friend std::ostream &operator<<(std::ostream &os, const e172::Vector &dt);

    friend bool operator<(const e172::Vector& vec0, const e172::Vector& vec1);

    static bool moduleLessComparator(const Vector &v0, const Vector &v1);

    void serialize(WriteBuffer &buf) const
    {
        buf.write(m_x);
        buf.write(m_y);
    }

    static std::optional<Vector> deserialize(ReadBuffer &buf)
    {
        Vector v;
        e172_chainingAssign(v.m_x, buf.read<double>());
        e172_chainingAssign(v.m_y, buf.read<double>());
        return v;
    }

    static std::optional<Vector> deserializeConsume(ReadBuffer &&buf) { return deserialize(buf); }

private:
    double m_x = 0;
    double m_y = 0;
};
} // namespace e172

#endif // VECTOR_H
