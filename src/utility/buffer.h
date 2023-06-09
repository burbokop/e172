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

class WriteBuffer;
class ReadBuffer;

/// TODO add check whether enum has explicit undeliying type
template<typename T>
concept SerializePrimitive = std::is_arithmetic<T>::value || std::is_enum<T>::value;

template<typename T>
concept Serialize = requires(T const v, WriteBuffer &buf)
{
    {v.serialize(buf)};
}
|| SerializePrimitive<T>;

/// TODO add check whether enum has explicit undeliying type
template<typename T>
concept DeserializePrimitive = std::is_arithmetic<T>::value || std::is_enum<T>::value;

template<typename T>
concept Deserialize = requires(ReadBuffer & buf, ReadBuffer &&tmpbuf)
{
    {
        T::deserialize(buf)
    } -> std::convertible_to<std::optional<T>>;

    {
        T::deserializeConsume(std::move(tmpbuf))
    } -> std::convertible_to<std::optional<T>>;
}
|| DeserializePrimitive<T>;

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

    std::size_t size() const { return m_data.size(); }

    std::size_t write(const Byte *bytes, std::size_t size)
    {
        m_data.insert(m_data.end(), bytes, bytes + size);
        return size;
    }

    std::size_t write(const Bytes &v) { return write(v.data(), v.size()); }

    std::size_t write(WriteBuffer &&b) { return write(b.m_data.data(), b.m_data.size()); }

    template<Serialize T>
    std::size_t write(const T &v)
    {
        if constexpr (SerializePrimitive<T>) {
            auto vCopy = v;
            auto ptr = reinterpret_cast<Byte *>(&vCopy);
            toBigEndian(ptr, sizeof(T));
            const auto cnt = write(ptr, sizeof(T));
            assert(cnt == sizeof(T));
            return cnt;
        } else {
            const auto s = size();
            v.serialize(*this);
            return size() - s;
        }
    }

    std::size_t writeDyn(const Bytes &v)
    {
        const auto s = size();
        write(std::uint32_t(v.size()));
        write(v.data(), v.size());
        return size() - s;
    }

    std::size_t writeDyn(const std::string &str)
    {
        const auto s = size();
        write(std::uint32_t(str.size()));
        write(reinterpret_cast<const Byte *>(str.data()), str.size());
        return size() - s;
    }

    static Bytes collect(WriteBuffer &&b) { return std::move(b.m_data); }

    template<Serialize T>
    static Bytes toBytes(const T &v)
    {
        WriteBuffer b;
        b.write(v);
        return b.m_data;
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

    std::size_t bytesAvailable() const { return m_data.size() - m_pos; }

    std::optional<Bytes> read(std::size_t size)
    {
        assert(m_valid);
        if (bytesAvailable() >= size) {
            auto result = Bytes(m_data.begin() + m_pos, m_data.begin() + m_pos + size);
            m_pos += size;
            return std::move(result);
        } else {
#ifndef NDEBUG
            m_valid = false;
#endif
            return std::nullopt;
        }
    }

    template<Deserialize T>
    std::optional<T> read()
    {
        assert(m_valid);
        if constexpr (DeserializePrimitive<T>) {
            const auto result = readPrimitive<T>();
#ifndef NDEBUG
            if (!result)
                m_valid = false;
#endif
            return result;
        } else {
            const auto result = T::deserialize(*this);
#ifndef NDEBUG
            if (!result)
                m_valid = false;
#endif
            return result;
        }
    }

    template<typename T>
        std::optional<T> readDyn() requires std::is_same<T, Bytes>::value
        || std::is_same<T, std::string>::value
    {
        if (const auto size = read<std::uint32_t>()) {
            if (const auto dynData = read(*size)) {
                if constexpr (std::is_same<T, Bytes>::value) {
                    return dynData;
                } else {
                    return std::string(dynData->begin(), dynData->end());
                }
            }
        }
        return std::nullopt;
    }

    static Bytes readAll(ReadBuffer &&p)
    {
        assert(p.m_valid);
        return Bytes(p.m_data.begin() + p.m_pos, p.m_data.end());
    }

    template<Deserialize T>
    static std::optional<T> consume(ReadBuffer &&p)
    {
        assert(p.m_valid);
        if constexpr (DeserializePrimitive<T>) {
            return p.readPrimitive<T>();
        } else {
            return T::deserializeConsume(p);
        }
    }

    template<Deserialize T>
    static std::optional<T> fromBytes(Bytes &&b)
    {
        return consume<T>(ReadBuffer(std::move(b)));
    }

private:
    template<DeserializePrimitive T>
    std::optional<T> readPrimitive()
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
#ifndef NDEBUG
    bool m_valid = true;
#endif
};

} // namespace e172
