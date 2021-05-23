#include "line2d.h"
#include "math.h"

e172::Line2d::Line2d(double a, double b, double c) : m_a(a), m_b(b), m_c(c) {}

double e172::Line2d::distanceToPoint(const e172::Vector& point) const {
    const auto& m = e172::Vector(m_a, m_b).module();
    if (m != Math::null) {
        return std::abs(m_a * point.x() + m_b * point.y() + m_c) / m;
    } else {
        return 0;
    }
}

e172::Line2d::Line2d() {}

double e172::Line2d::c() const {
    return m_c;
}

bool e172::Line2d::isValid() const {
    return !(m_a == Math::null && m_b == Math::null);
}

e172::Line2d e172::Line2d::fromTan(double tan, const e172::Vector &point) {
    return Line2d(-tan, 1, tan * point.x() - point.y());
}

e172::Line2d e172::Line2d::fromAngle(double angle, const e172::Vector &point) {
    angle = Math::constrainRadians(angle);
    if ((angle - Math::Pi / 2) == Math::null || (angle - Math::Pi * 3 / 2) == Math::null) {
        return Line2d(1, 0, -point.x());
    } else {
        return fromTan(std::tan(angle), point);
    }
}

e172::Line2d e172::Line2d::fromVector(const e172::Vector &vector, const e172::Vector &point) {
    if (vector.x() != Math::null) {
        return fromTan(vector.y() / vector.x(), point);
    } else {
        return Line2d();
    }
}

double e172::Line2d::x(double y) const {
    if(m_a != Math::null) {
        return -(m_b * y + m_c) / m_a;
    } else {
        return 0;
    }
}

double e172::Line2d::y(double x) const {
    if(m_b != Math::null) {
        return -(m_a * x + m_c) / m_b;
    } else {
        return 0;
    }
}

std::pair<e172::Vector, e172::Vector> e172::Line2d::rectIntersection(const e172::Vector &size) const {
    if(m_b != Math::null) {
        const auto& x0 = 0;
        const auto& x1 = size.x();
        const auto& y0 = y(x0);
        const auto& y1 = y(x1);
        return { e172::Vector(x0, y0), e172::Vector(x1, y1) };
    } else {
        const auto& y0 = 0;
        const auto& y1 = size.y();
        const auto& x0 = x(y0);
        const auto& x1 = x(y1);
        return { e172::Vector(x0, y0), e172::Vector(x1, y1) };
    }
}

e172::Line2d e172::Line2d::tanslated(const e172::Vector &offset) const {
    return Line2d(m_a, m_b, m_c - (m_a * offset.x() + m_b * offset.y()));
}

double e172::Line2d::b() const {
    return m_b;
}

double e172::Line2d::a() const {
    return m_a;
}

std::ostream &operator<<(std::ostream &stream, const e172::Line2d &line) {
    return stream << "line(" << line.a() << ", " << line.b() << ", " << line.c() << ")";
}
