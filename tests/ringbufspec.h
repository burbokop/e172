#pragma once

#include <src/testing.h>

namespace e172::tests {

class RingBufSpec
{
    static void untilFullTest() e172_test(RingBufSpec, untilFullTest);
    static void pushPopTest() e172_test(RingBufSpec, pushPopTest);
    static void topTest() e172_test(RingBufSpec, topTest);
    static void peekTest() e172_test(RingBufSpec, peekTest);
    static void peekNotAllTest() e172_test(RingBufSpec, peekNotAllTest);
};

} // namespace e172::tests
