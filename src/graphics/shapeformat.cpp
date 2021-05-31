#include "shapeformat.h"

bool e172::ShapeFormat::fill() const {
    return m_fill;
}

e172::ShapeFormat::ShapeFormat(bool fill) : m_fill(fill) {}
