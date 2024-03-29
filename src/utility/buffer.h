// Copyright 2023 Borys Boiko

#pragma once

#include "../todo.h"
#include "../traits.h"
#include <assert.h>
#include <bit>
#include <cstdint>
#include <functional>
#include <iomanip>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
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

/**
 * @brief flipEndian - inverts bytes `p` with size `s` if current platform endian is little
 * If you write it means "to big endian"
 * If you read it means "from big endian"
 * @param p
 * @param s
 */
constexpr void flipEndian(Byte *p, std::size_t s)
{
    if constexpr (std::endian::native == std::endian::little) {
        for (std::size_t i = 0; i < s / 2; ++i) {
            const auto tmp = p[i];
            p[i] = p[s - i - 1];
            p[s - i - 1] = tmp;
        }
    }
}

/**
 * @brief The WriteBuffer class provides serialization of any type into platform independent bytes
 */
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
            flipEndian(ptr, sizeof(T));
            const auto cnt = write(ptr, sizeof(T));
            assert(cnt == sizeof(T));
            return cnt;
        } else {
            const auto s = size();
            v.serialize(*this);
            return size() - s;
        }
    }

    template<typename T>
    std::size_t writeWithLoss(const std::optional<T> &v)
        requires std::is_integral<T>::value || std::is_enum<T>::value
    {
        if (v) {
            return write<T>(*v + 1);
        } else {
            return write<T>(0);
        }
    }

    std::size_t writeDyn(const Bytes &v)
    {
        const auto s = size();
        write(std::uint32_t(v.size()));
        write(v.data(), v.size());
        return size() - s;
    }

    std::size_t writeDyn(WriteBuffer &&b) { return writeDyn(b.m_data); }

    std::size_t writeDyn(const std::string &str)
    {
        const auto s = size();
        write(std::uint32_t(str.size()));
        write(reinterpret_cast<const Byte *>(str.data()), str.size());
        return size() - s;
    }

    /**
     * @brief writeDyn - write `count` of serializable elements with different sizes
     * @param count - count of elements to serialize
     * @param each - called `count` times to serialize each element 
     * @return 
     */
    std::size_t writeDyn(std::size_t count,
                         const std::function<void(std::size_t i, WriteBuffer &)> &each)
    {
        const auto s = size();
        write<std::uint32_t>(static_cast<std::uint32_t>(count));
        for (std::size_t i = 0; i < count; ++i) {
            WriteBuffer tmp;
            each(i, tmp);
            writeDyn(std::move(tmp));
        }
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

    inline friend std::ostream &operator<<(std::ostream &stream, const WriteBuffer &buf)
    {
        return stream << buf.m_data;
    }

private:
    Bytes m_data;
};

/**
 * @brief The ReadBuffer class provides deserialization platform independent bytes into any type
 */
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
            return result;
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

    /**
     * Example:
     * .readWithLoss<std::optional<int>>()
     */
    template<typename T>
    std::optional<T> readWithLoss()
        requires std::is_integral<typename T::value_type>::value
                 || std::is_enum<typename T::value_type>::value
    {
        if (const auto v = read<typename T::value_type>()) {
            if (*v == 0) {
                if constexpr (IsOptional<T>::value) {
                    return T(std::nullopt);
                } else {
                    todo();
                }
            } else {
                return T(*v - 1);
            }
        }
        return std::nullopt;
    }

    /**
     * @brief readDyn - read dynamic sized value (bytes, string, or another buffer)
     * @note value by this index MUST be written by writeDyn<T>()
     */
    template<typename T>
    std::optional<T> readDyn()
        requires std::is_same<T, Bytes>::value || std::is_same<T, ReadBuffer>::value
                 || std::is_same<T, std::string>::value
    {
        if (const auto size = read<std::uint32_t>()) {
            if (auto dynData = read(*size)) {
                if constexpr (std::is_same<T, Bytes>::value) {
                    return dynData;
                } else if constexpr (std::is_same<T, ReadBuffer>::value) {
                    return ReadBuffer(std::move(*dynData));
                } else {
                    return std::string(dynData->begin(), dynData->end());
                }
            }
        }
        return std::nullopt;
    }

    /**
     * @brief readDyn - reads dynamic list of deserializable values with variadic sizes
     * @note value by this index MUST be written by writeDyn(std::size_t, std::function<void(std::size_t, WriteBuffer&)>)
     * @param each - will be called for each value to deserialize
     * @return number of elements read or nullopt on error
     */
    std::optional<std::size_t> readDyn(const std::function<bool(std::size_t i, ReadBuffer &&)> &each)
    {
        const auto &count = read<std::uint32_t>();
        if (!count) {
            return std::nullopt;
        }
        std::size_t i = 0;
        for (; i < static_cast<std::size_t>(*count); ++i) {
            auto dynData = readDyn<ReadBuffer>();
            if (!dynData) {
                return std::nullopt;
            }
            if (!each(i, std::move(*dynData))) {
                return std::nullopt;
            }
        }
        return i;
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

    inline friend std::ostream &operator<<(std::ostream &stream, const ReadBuffer &buf)
    {
        return stream << buf.m_data;
    }

private:
    template<DeserializePrimitive T>
    std::optional<T> readPrimitive()
    {
        if (bytesAvailable() >= sizeof(T)) {
            flipEndian(m_data.data() + m_pos, sizeof(T));
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

class ByteRange
{
public:
    constexpr ByteRange(const Byte *begin, const Byte *end)
        : m_begin(begin)
        , m_end(end)
    {}

    constexpr std::size_t size() const { return m_end - m_begin; }
    constexpr const Byte *begin() const { return m_begin; }
    constexpr const Byte *end() const { return m_end; }

    Bytes bytes() const { return Bytes(m_begin, m_end); }
    ReadBuffer buffer() const { return ReadBuffer(bytes()); }

private:
    const Byte *m_begin;
    const Byte *m_end;
};

} // namespace e172

#define e172_chainingAssignOrElse(OUTPUT, OPT, ELSE) \
    { \
        if (const auto &opt = OPT) { \
            OUTPUT = *opt; \
        } else { \
            return ELSE; \
        } \
    }

#define e172_chainingAssign(OUTPUT, OPT) e172_chainingAssignOrElse(OUTPUT, OPT, std::nullopt)
