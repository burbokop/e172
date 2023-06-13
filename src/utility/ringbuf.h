#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace e172 {

template<typename T, std::size_t CAPACITY>
struct RingBuf
{
public:
    RingBuf() = default;

    bool push(T v)
    {
        if (is_full()) {
            return false;
        } else {
            m_buf[m_end] = std::move(v);
            m_end = (m_end + 1) % CAPACITY;
            return true;
        }
    }

    std::optional<T> pop()
    {
        if (is_empty()) {
            return std::nullopt;
        } else {
            const auto v = std::move(m_buf[m_begin]);
            m_begin = (m_begin + 1) % CAPACITY;
            return v;
        }
    }

    std::optional<T> top() const requires std::is_copy_constructible<T>::value
    {
        if (is_empty()) {
            return std::nullopt;
        } else {
            return m_buf[m_begin];
        }
    }

    std::size_t peek(T *output, std::size_t size) const requires std::is_copy_constructible<T>::value
    {
        auto fb = m_begin;
        const auto len = std::min(size, this->len());
        for (std::size_t i = 0; i < len; ++i) {
            output[i] = m_buf[fb];
            fb = (fb + 1) % CAPACITY;
        }
        return len;
    }

    bool is_full() const { return push_ability() == 0; }

    bool is_empty() const { return m_begin == m_end; }

    std::size_t len() const { return CAPACITY - push_ability() - 1; }

    std::size_t push_ability() const { return (CAPACITY + m_begin - m_end - 1) % CAPACITY; }

private:
    T m_buf[CAPACITY];
    std::size_t m_begin = 0;
    std::size_t m_end = 0;
};

} // namespace e172
