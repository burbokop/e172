#pragma once

#include "buffer.h"
#include "io.h"

namespace e172 {

using PackageLen = std::uint32_t;
using PackageType = std::uint16_t;

class WritePackage
{
public:
    std::size_t write(const Byte *bytes, std::size_t size) { return m_buf.write(bytes, size); }
    std::size_t write(const Bytes &v) { return m_buf.write(v.data(), v.size()); }
    std::size_t write(WriteBuffer &&b) { return m_buf.write(std::move(b)); }

    template<Serialize T>
    std::size_t write(const T &v)
    {
        return m_buf.write(v);
    }

    std::size_t writeDyn(const Bytes &v) { return m_buf.writeDyn(v); }
    std::size_t writeDyn(const std::string &str) { return m_buf.writeDyn(str); }

    static std::size_t push(Write &dst,
                            PackageType type,
                            const std::function<void(WritePackage)> &writeFn)
    {
        WriteBuffer tmp;
        writeFn(WritePackage(tmp));

        WriteBuffer result;
        result.write(PackageLen(tmp.size()));
        result.write(type);
        result.write(std::move(tmp));
        return dst.write(std::move(result));
    }

private:
    WritePackage(WriteBuffer &buf)
        : m_buf(buf)
    {}

    WriteBuffer &m_buf;
};

class ReadPackage
{
public:

    std::size_t bytesAvailable() const { return m_buf.bytesAvailable(); }

    std::optional<Bytes> read(std::size_t size) { return m_buf.read(size); }

    template<typename T>
    std::optional<T> read()
    {
        return m_buf.read<T>();
    }

    template<typename T>
        std::optional<T> readDyn() requires std::is_same<T, Bytes>::value
        || std::is_same<T, std::string>::value
    {
        return m_buf.readDyn<T>();
    }

    PackageType type() const { return m_type; };

    static std::size_t pull(Read &r, const std::function<void(ReadPackage)> &readFn)
    {
        r.bufferize();
        if (const auto len = r.peek<PackageLen>()) {
            const auto packageSize = sizeof(PackageLen) + sizeof(PackageType) + *len;
            if (r.bytesAvailable() >= packageSize) {
                const auto ok = r.read<PackageLen>();
                assert(ok);
                const auto type = r.read<PackageType>();
                assert(type);
                auto result = r.read<ReadBuffer>(*len);
                assert(result.bytesAvailable() == *len);
                readFn(ReadPackage(type.right().value(), std::move(result)));
                return packageSize;
            }
        }
        return 0;
    }

    template<typename T>
        static T readAll(ReadPackage &&p) requires std::is_same<T, Bytes>::value
        || std::is_same<T, ReadBuffer>::value
    {
        if constexpr (std::is_same<T, Bytes>::value) {
            return ReadBuffer::readAll(std::move(p.m_buf));
        } else {
            return std::move(p.m_buf);
        }
    }

    template<Deserialize T>
    static std::optional<T> consume(ReadPackage &&p)
    {
        return ReadBuffer::consume<T>(std::move(p.m_buf));
    }

private:
    ReadPackage(PackageType type, ReadBuffer buf)
        : m_type(type)
        , m_buf(std::move(buf))
    {}

private:
    PackageType m_type;
    ReadBuffer m_buf;
};

class UnknownPackageTypeException : public std::exception
{
public:
    UnknownPackageTypeException(PackageType type)
        : m_message(std::string("Unknown package type: ") + std::to_string(type)){};

    // exception interface
public:
    const char *what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace e172
