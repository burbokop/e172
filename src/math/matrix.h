#pragma once

#include "../utility/buffer.h"
#include "vector.h"

namespace e172 {

class Matrix {
public:
    static Matrix identity() { return Matrix(1, 0, 0, 1); }
    static Matrix scale(double x, double y) { return Matrix(x, 0, 0, y); }
    static Matrix scale(double v) { return Matrix(v, 0, 0, v); }
    static Matrix fromRadians(double value);

    Vector operator*(const Vector &vector) const
    {
        return {m_a11 * vector.x() + m_a12 * vector.y(), m_a21 * vector.x() + m_a22 * vector.y()};
    }

    void serialize(WriteBuffer &buf) const
    {
        buf.write(m_a11);
        buf.write(m_a12);
        buf.write(m_a21);
        buf.write(m_a22);
    }

    static std::optional<Matrix> deserialize(ReadBuffer &buf)
    {
        Matrix m;
        e172_chainingAssign(m.m_a11, buf.read<double>());
        e172_chainingAssign(m.m_a12, buf.read<double>());
        e172_chainingAssign(m.m_a21, buf.read<double>());
        e172_chainingAssign(m.m_a22, buf.read<double>());
        return m;
    }

    static std::optional<Matrix> deserializeConsume(ReadBuffer &&buf) { return deserialize(buf); }

private:
    Matrix() = default;
    Matrix(double a11, double a12, double a21, double a22)
        : m_a11(a11)
        , m_a12(a12)
        , m_a21(a21)
        , m_a22(a22)
    {}

private:
    double m_a11 = 0;
    double m_a12 = 0;
    double m_a21 = 0;
    double m_a22 = 0;
};

} // namespace e172
