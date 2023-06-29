// Copyright 2023 Borys Boiko

#pragma once

#include "../../src/testing.h"

namespace e172::tests {

class RingBufSpec
{
    static void untilFullTest() e172_test(RingBufSpec, untilFullTest);
    static void pushPopTest() e172_test(RingBufSpec, pushPopTest);
    static void topTest() e172_test(RingBufSpec, topTest);
    static void peekTest() e172_test(RingBufSpec, peekTest);
    static void peekNotAllTest() e172_test(RingBufSpec, peekNotAllTest);
    static void streamTestWithCapacity5() e172_test(RingBufSpec, streamTestWithCapacity5);
    static void streamTestWithCapacity6() e172_test(RingBufSpec, streamTestWithCapacity6);
    static void streamTestWithCapacity7() e172_test(RingBufSpec, streamTestWithCapacity7);
    static void streamTestWithCapacity8() e172_test(RingBufSpec, streamTestWithCapacity8);
    static void streamTestWithCapacity9() e172_test(RingBufSpec, streamTestWithCapacity9);
    static void streamTestWithCapacity15() e172_test(RingBufSpec, streamTestWithCapacity15);
    static void streamTestWithCapacity16() e172_test(RingBufSpec, streamTestWithCapacity16);
    static void streamTestWithCapacity111() e172_test(RingBufSpec, streamTestWithCapacity111);
};

} // namespace e172::tests
