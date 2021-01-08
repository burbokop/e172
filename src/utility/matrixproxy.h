#ifndef MATRIXPROXY_H
#define MATRIXPROXY_H

#include <stdlib.h>

namespace e172 {


template<typename T>
class MatrixProxy {
    T *m_data = nullptr;
    size_t m_width = 0;
    size_t m_height = 0;
public:
    MatrixProxy() {}
    MatrixProxy(size_t width, size_t height, T *data) {
        m_width = width;
        m_height = height;
        m_data = data;
    }

    T *data() const { return m_data; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    T& value(size_t x, size_t y) { return m_data[y * m_width + x]; }
    T value(size_t x, size_t y) const { return m_data[y * m_width + x]; }
};

}
#endif // MATRIXPROXY_H
