#pragma once

#include "src/todo.h"
#include <assert.h>
#include <bit>
#include <cstdint>
#include <functional>
#include <vector>

namespace e172 {

using Byte = std::uint8_t;
using Bytes = std::vector<std::uint8_t>;

class WriteBuffer
{
public:
    static constexpr void toBigEndian(Byte *p, std::size_t s)
    {
        if constexpr (std::endian::native == std::endian::little) {
            for (std::size_t i = 0; i < s / 2; ++i) {
                const auto tmp = p[i];
                p[i] = p[s - i - 1];
                p[s - i - 1] = tmp;
            }
        }
    }

    std::size_t write(const Byte *bytes, std::size_t size)
    {
        m_data.insert(m_data.end(), bytes, bytes + size);
        return size;
    }

    template<typename T>
    std::size_t write(const T &v)
    {
        auto vCopy = v;
        auto ptr = reinterpret_cast<Byte *>(&vCopy);
        toBigEndian(ptr, sizeof(T));
        const auto cnt = write(ptr, sizeof(T));
        assert(cnt == sizeof(T));
        return cnt;
    }

    std::size_t writeBuf(const WriteBuffer &b) { return write(b.m_data.data(), b.m_data.size()); }

    std::size_t size() const { return m_data.size(); }

    static std::vector<std::uint8_t> collect(WriteBuffer &&b) { return std::move(b.m_data); }

    template<typename T>
    static Bytes toBytes(const T &v)
    {
        todo;
    }

private:
    std::vector<std::uint8_t> m_data;
};

class ReadBuffer
{
public:
    template<typename T>
    static T fromBytes(const Bytes &b)
    {
        todo;
    }
};

class WritePackage
{
public:
    std::size_t write(const Byte *bytes, std::size_t size)
    {
        m_buf.write(bytes, size);
        return size;
    }

    template<typename T>
    std::size_t write(const T &v)
    {
        return m_buf.write(v);
    }

    std::size_t writeBuf(const WriteBuffer &b) { return m_buf.writeBuf(b); }

    friend Bytes pack(std::function<void(WritePackage)> writeFn);

private:
    WritePackage(WriteBuffer &buf)
        : m_buf(buf)
    {}

    WriteBuffer &m_buf;
};

inline Bytes pack(std::function<void(WritePackage)> writeFn)
{
    WriteBuffer tmp;
    writeFn(WritePackage(tmp));

    WriteBuffer result;
    result.write(std::uint32_t(tmp.size()));
    result.write(tmp);

    return std::move(WriteBuffer::collect(std::move(result)));
}

} // namespace e172
