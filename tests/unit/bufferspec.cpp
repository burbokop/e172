// Copyright 2023 Borys Boiko

#include "bufferspec.h"

#include "../../src/utility/buffer.h"
#include <string>
#include <utility>

namespace e172::tests {

void BufferSpec::writeBufferTest()
{
    WriteBuffer buf;

    buf.write<std::uint32_t>(12);
    buf.write<std::uint8_t>(12);

    const auto bytes = WriteBuffer::collect(std::move(buf));
    e172_shouldEqual(bytes, e172_initializerList(Bytes, 0, 0, 0, 12, 12));
}

void BufferSpec::writeBufferToBufferTest()
{
    WriteBuffer buf;
    buf.write<std::uint8_t>(12);

    buf.write([] {
        WriteBuffer innerBuf;
        innerBuf.write<std::uint16_t>(6);
        innerBuf.write<std::uint16_t>(5);
        return innerBuf;
    }());
    buf.write<std::uint8_t>(12);
    buf.write(Bytes{1, 2});
    buf.write<std::uint8_t>(3);

    const auto bytes = WriteBuffer::collect(std::move(buf));
    e172_shouldEqual(bytes, e172_initializerList(Bytes, 12, 0, 6, 0, 5, 12, 1, 2, 3));
}

void BufferSpec::readBufferTest()
{
    ReadBuffer buf({1, 2, 3, 4, 5, 6, 7, 8, 9});

    e172_shouldEqual(buf.bytesAvailable(), 9);
    e172_shouldEqual(buf.read<std::uint32_t>().value(), 16909060);
    e172_shouldEqual(buf.bytesAvailable(), 5);
    e172_shouldEqual(buf.read<std::uint16_t>().value(), 1286);
    e172_shouldEqual(buf.bytesAvailable(), 3);
    e172_shouldEqual(ReadBuffer::readAll(std::move(buf)), e172_initializerList(Bytes, 7, 8, 9))
}

void BufferSpec::readWriteDynStr()
{
    WriteBuffer write;
    write.writeDyn("gogadoda");
    ReadBuffer read = WriteBuffer::collect(std::move(write));
    e172_shouldEqual(read.readDyn<std::string>().value(), "gogadoda");
}

void BufferSpec::readWriteDynBytes()
{
    WriteBuffer write;
    write.writeDyn(Bytes{0, 1, 2, 3, 4});
    ReadBuffer read = WriteBuffer::collect(std::move(write));
    e172_shouldEqual(read.readDyn<Bytes>().value(), e172_initializerList(Bytes, 0, 1, 2, 3, 4));
}

void BufferSpec::readWriteDynList()
{
    WriteBuffer write;
    write.writeDyn(3, [](std::size_t i, WriteBuffer &current) {
        switch (i) {
        case 0:
            current.write<std::uint32_t>(12);
            break;
        case 1:
            current.writeDyn("goga");
            break;
        case 2:
            current.write<std::uint32_t>(2);
            current.write<std::uint32_t>(4);
            break;
        }
    });
    ReadBuffer read = WriteBuffer::collect(std::move(write));

    const auto count = read.readDyn([](std::size_t i, ReadBuffer &&current) -> bool {
        switch (i) {
        case 0:
            e172_shouldEqual(current.read<std::uint32_t>().value(), 12);
            break;
        case 1:
            e172_shouldEqual(current.readDyn<std::string>().value(), "goga");
            break;
        case 2:
            e172_shouldEqual(current.read<std::uint32_t>().value(), 2);
            e172_shouldEqual(current.read<std::uint32_t>().value(), 4);
            break;
        }
        return true;
    });
    e172_shouldEqual(count.value(), 3);
}

} // namespace e172::tests
