#pragma once

#include <tuple>
#include <stdlib.h>
#include <src/testing.h>

namespace e172 {
class Variant;
}

namespace e172::benches {

class VariantBenches
{
    static void banchmark() e172_test(VariantSpec, banchmark);

    static std::pair<int64_t, int64_t> speedTest(size_t count);
    static double speedTest();
};

} // namespace e172::benches
