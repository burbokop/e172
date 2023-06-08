#pragma once

#include <tuple>
#include <stdlib.h>
#include <src/testing.h>

namespace e172 {
class Variant;
}

namespace e172::tests {

class VariantSpec
{
    static void compareTest0() e172_test(VariantSpec, compareTest0);
    static void compareTest1() e172_test(VariantSpec, compareTest1);

    static void fromJsonTest0() e172_test(VariantSpec, fromJsonTest0);
    static void fromJsonTest1() e172_test(VariantSpec, fromJsonTest1);

    static void banchmark() e172_test(VariantSpec, banchmark);

    static std::pair<int64_t, int64_t> speedTest(size_t count);
    static double speedTest();
};

} // namespace e172::tests
