// Copyright 2023 Borys Boiko

#pragma once

namespace e172 {

class ShapeFormat {
    bool m_fill = false;
public:
    ShapeFormat(bool fill);
    bool fill() const { return m_fill; }
};

}
