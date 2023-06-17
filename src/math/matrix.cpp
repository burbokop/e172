// Copyright 2023 Borys Boiko

#include "matrix.h"

#include <cmath>

e172::Matrix e172::Matrix::fromRadians(double value) {
    Matrix m;
    m.m_a11 = std::cos(value);
    m.m_a12 = -std::sin(value);
    m.m_a21 = -m.m_a12;
    m.m_a22 = m.m_a11;
    return m;
}
