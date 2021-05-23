#ifndef LINE_H
#define LINE_H

#include "vector.h"


namespace e172 {

class Line2d {
    double m_a = 0, m_b = 0, m_c = 0;
public:
    Line2d();
    Line2d(double a, double b, double c);

    double distanceToPoint(const e172::Vector& point) const;
    double a() const;
    double b() const;
    double c() const;
    bool isValid() const;

    static Line2d fromTan(double tan, const e172::Vector& point = e172::Vector());
    static Line2d fromAngle(double angle, const e172::Vector& point = e172::Vector());
    static Line2d fromVector(const e172::Vector& vector, const e172::Vector& point = e172::Vector());

    double operator()(double x) const { return y(x); }
    double x(double y) const;
    double y(double x) const;
    std::pair<e172::Vector, e172::Vector> rectIntersection(const e172::Vector& size) const;

    Line2d tanslated(const e172::Vector& offset) const;
};

}

std::ostream &operator<<(std::ostream& stream, const e172::Line2d& line);

#endif // LINE_H
