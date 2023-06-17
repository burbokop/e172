// Copyright 2023 Borys Boiko

#pragma once

#include <vector>

namespace e172 {

template<typename T>
class VectorProxy {
public:
    VectorProxy(const std::vector<std::vector<T>*> &pointers) {
        m_pointers = pointers;
    }

    std::size_t size() const
    {
        std::size_t sum = 0;
        for (auto p : m_pointers) {
            sum += p->size();
        }
        return sum;
    }

    T &operator[](std::size_t index)
    {
        std::size_t pi = 0;
        while (index >= m_pointers[pi]->size()) {
            index -= m_pointers[pi]->size();
            ++pi;
        }
        return (*m_pointers[pi])[index];
    }

    T operator[](std::size_t index) const
    {
        std::size_t pi = 0;
        while (index >= m_pointers[pi]->size()) {
            index -= m_pointers[pi]->size();
            ++pi;
        }
        return (*m_pointers[pi])[index];
    }

private:
    std::vector<std::vector<T> *> m_pointers;
};

} // namespace e172
