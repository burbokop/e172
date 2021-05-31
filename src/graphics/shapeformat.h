#ifndef SHAPEFORMAT_H
#define SHAPEFORMAT_H

namespace e172 {

class ShapeFormat {
    bool m_fill = false;
public:
    ShapeFormat(bool fill);
    bool fill() const;
};

}

#endif // SHAPEFORMAT_H
