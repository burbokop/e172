// Copyright 2023 Borys Boiko

#pragma once

#include "../../src/testing.h"

namespace e172::tests {

class BufferSpec
{
    static void writeBufferTest() e172_test(BufferSpec, writeBufferTest);
    static void writeBufferToBufferTest() e172_test(BufferSpec, writeBufferToBufferTest);
    static void readBufferTest() e172_test(BufferSpec, readBufferTest);
    static void readWriteDynStr() e172_test(BufferSpec, readWriteDynStr);
    static void readWriteDynBytes() e172_test(BufferSpec, readWriteDynBytes);
    static void readWriteDynList() e172_test(BufferSpec, readWriteDynList);
};

} // namespace e172::tests
