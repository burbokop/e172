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
};

} // namespace e172::tests
