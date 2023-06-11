#include "bufferspec.h"

#include <src/utility/buffer.h>

void e172::tests::BufferSpec::writeBufferTest()
{
    WriteBuffer buf;

    buf.write<std::uint32_t>(12);
    buf.write<std::uint8_t>(12);

    const auto bytes = WriteBuffer::collect(std::move(buf));
    e172_shouldEqual(bytes, e172_initializerList(Bytes, 0, 0, 0, 12, 12));
}

void e172::tests::BufferSpec::writeBufferToBufferTest()
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

void e172::tests::BufferSpec::readBufferTest()
{
    ReadBuffer buf({1, 2, 3, 4, 5, 6, 7, 8, 9});

    e172_shouldEqual(buf.bytesAvailable(), 9);
    e172_shouldEqual(buf.read<std::uint32_t>().value(), 16909060);
    e172_shouldEqual(buf.bytesAvailable(), 5);
    e172_shouldEqual(buf.read<std::uint16_t>().value(), 1286);
    e172_shouldEqual(buf.bytesAvailable(), 3);
    e172_shouldEqual(ReadBuffer::readAll(std::move(buf)), e172_initializerList(Bytes, 7, 8, 9))
}
