#pragma once

#include "buffer.h"
#include "either.h"
#include <optional>

namespace e172 {

/**
 * @brief The Read class - abstract read device (file, socket, pipe, ...)
 */
class Read
{
public:
    Read() = default;
    virtual ~Read() = default;

    virtual std::size_t bufferize() = 0;
    virtual std::size_t bytesAvailable() const = 0;
    virtual std::size_t read(Byte *dst, std::size_t size) = 0;
    virtual std::size_t peek(Byte *dst, std::size_t size) const = 0;

    template<typename T = Bytes>
    T read(std::size_t s) requires std::is_convertible<Bytes, T>::value
    {
        Bytes b;
        b.resize(s);
        b.resize(read(b.data(), b.size()));
        return b;
    }

    template<typename T>
    std::optional<T> peek() const
    {
        Bytes b;
        b.resize(sizeof(T));
        const auto bytesCount = peek(b.data(), b.size());
        if (bytesCount == sizeof(T)) {
            return ReadBuffer::fromBytes<T>(std::move(b));
        } else {
            return std::nullopt;
        }
    }

    template<typename T>
    Either<Bytes, T> read()
    {
        Bytes b;
        b.resize(sizeof(T));
        const auto bytesCount = read(b.data(), b.size());
        if (bytesCount == sizeof(T)) {
            return Right(ReadBuffer::fromBytes<T>(std::move(b)).value());
        } else {
            return Left(b);
        }
    }
};

/**
 * @brief The Write class - abstract write device (file, socket, pipe, ...)
 */
class Write
{
public:
    Write() = default;
    virtual ~Write() = default;

    virtual std::size_t write(const Byte *bytes, std::size_t size) = 0;
    virtual void flush() = 0;

    std::size_t write(const Bytes &bytes) { return write(bytes.data(), bytes.size()); }

    std::size_t write(WriteBuffer &&buf)
    {
        const auto bytes = WriteBuffer::collect(std::move(buf));
        return write(bytes.data(), bytes.size());
    }
};

} // namespace e172
