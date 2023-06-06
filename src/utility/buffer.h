#pragma once

#include "src/todo.h"
#include <assert.h>
#include <bit>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <optional>
#include <ostream>
#include <vector>

namespace e172 {

using Byte = std::uint8_t;

class Bytes : public std::vector<std::uint8_t>
{
public:
    using std::vector<std::uint8_t>::vector;

    inline friend std::ostream &operator<<(std::ostream &stream, const Bytes &bytes)
    {
        std::size_t i = 0;
        for (const auto &b : bytes) {
            stream << std::hex << std::setfill('0') << std::setw(2) << int(b);
            if (i++ < bytes.size() - 1) {
                stream << '.';
            }
        }
        return stream;
    }
};

constexpr void toBigEndian(Byte *p, std::size_t s)
{
    if constexpr (std::endian::native == std::endian::little) {
        for (std::size_t i = 0; i < s / 2; ++i) {
            const auto tmp = p[i];
            p[i] = p[s - i - 1];
            p[s - i - 1] = tmp;
        }
    }
}

class WriteBuffer
{
public:
    WriteBuffer() = default;
    WriteBuffer(WriteBuffer &&) = default;
    WriteBuffer(const WriteBuffer &) = delete;

    std::size_t write(const Byte *bytes, std::size_t size)
    {
        m_data.insert(m_data.end(), bytes, bytes + size);
        return size;
    }

    template<typename T>
    std::size_t write(const T &v)
    {
        static_assert(!std::is_same<T, WriteBuffer>::value,
                      "T can not be WriteBuffer. use WriteBuffer::writeBuf");

        if constexpr (std::is_same<T, Bytes>::value) {
            return write(v.data(), v.size());
        } else {
            auto vCopy = v;
            auto ptr = reinterpret_cast<Byte *>(&vCopy);
            toBigEndian(ptr, sizeof(T));
            const auto cnt = write(ptr, sizeof(T));
            assert(cnt == sizeof(T));
            return cnt;
        }
    }

    std::size_t writeBuf(WriteBuffer &&b) { return write(b.m_data.data(), b.m_data.size()); }

    std::size_t size() const { return m_data.size(); }

    static Bytes collect(WriteBuffer &&b) { return std::move(b.m_data); }

    template<typename T>
    static Bytes toBytes(const T &v)
    {
        todo;
    }

private:
    Bytes m_data;
};

class ReadBuffer
{
public:
    ReadBuffer(Bytes &&b)
        : m_data(std::move(b))
    {}
    ReadBuffer(ReadBuffer &&) = default;
    ReadBuffer(const ReadBuffer &) = delete;

    template<typename T>
    static std::optional<T> fromBytes(Bytes &&b)
    {
        ReadBuffer r(std::move(b));
        return r.read<T>();
    }

    std::size_t bytesAvailable() const { return m_data.size() - m_pos; }

    static Bytes readAll(ReadBuffer &&p)
    {
        return Bytes(p.m_data.begin() + p.m_pos, p.m_data.end());
    }

    std::optional<Bytes> read(std::size_t size)
    {
        if (bytesAvailable() >= size) {
            auto result = Bytes(m_data.begin() + m_pos, m_data.begin() + m_pos + size);
            m_pos += size;
            return std::move(result);
        } else {
            return std::nullopt;
        }
    }

    template<typename T>
    std::optional<T> read()
    {
        if (bytesAvailable() >= sizeof(T)) {
            toBigEndian(m_data.data() + m_pos, sizeof(T));
            const auto ptr = reinterpret_cast<const T *>(m_data.data() + m_pos);
            m_pos += sizeof(T);
            return *ptr;
        } else {
            return std::nullopt;
        }
    }

private:
    Bytes m_data;
    std::size_t m_pos = 0;
};

} // namespace e172
