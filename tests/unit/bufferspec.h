#pragma once

#include <src/testing.h>

namespace e172::tests {

class BufferSpec
{
    static void writeBufferTest() e172_test(BufferSpec, writeBufferTest);
    static void writeBufferToBufferTest() e172_test(BufferSpec, writeBufferToBufferTest);
    static void readBufferTest() e172_test(BufferSpec, readBufferTest);
};

} // namespace e172::tests
