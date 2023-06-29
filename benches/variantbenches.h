// Copyright 2023 Borys Boiko

#pragma once

#include "../src/testing.h"
#include <stdlib.h>
#include <tuple>
#include <utility>

namespace e172 {
class Variant;
}

namespace e172::benches {

class VariantBenches
{
    static void banchmark() e172_test(VariantBenches, banchmark);

    static std::pair<int64_t, int64_t> speedTest(size_t count);
    static double speedTest();
};

} // namespace e172::benches
