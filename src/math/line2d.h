// Copyright 2023 Borys Boiko

#pragma once

#include "vector.h"
#include <utility>

namespace e172 {

class Line2d {
public:
    constexpr Line2d() = default;

    constexpr Line2d(double a, double b, double c)
        : m_a(a)
        , m_b(b)
        , m_c(c)
    {}

    double distanceToPoint(const Vector<double> &point) const;
    constexpr double a() const { return m_a; }
    constexpr double b() const { return m_b; }
    constexpr double c() const { return m_c; }
    bool isValid() const;

    static Line2d fromTan(double tan, const Vector<double> &point = {});
    static Line2d fromAngle(double angle, const Vector<double> &point = {});
    static Line2d fromVector(const Vector<double> &vector, const Vector<double> &point = {});

    double operator()(double x) const { return y(x); }
    double x(double y) const;
    double y(double x) const;
    std::pair<Vector<double>, Vector<double>> rectIntersection(const Vector<double> &size) const;

    Line2d tanslated(const Vector<double> &offset) const;

    inline friend std::ostream &operator<<(std::ostream &stream, const Line2d &line)
    {
        return stream << "line(" << line.a() << ", " << line.b() << ", " << line.c() << ")";
    }

private:
    double m_a = 0, m_b = 0, m_c = 0;
};

} // namespace e172
