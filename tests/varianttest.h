#ifndef VARIANTTEST_H
#define VARIANTTEST_H

#include <tuple>
#include <stdlib.h>
#include <src/testing.h>

namespace e172 {


class VariantTest {
    static void compareTest0(); e172_test(compareTest0)
    static void compareTest1(); e172_test(compareTest1)

    static void fromJsonTest0(); e172_test(fromJsonTest0)
    static void fromJsonTest1(); e172_test(fromJsonTest1)

    static void banchmark(); e172_test(banchmark)

    static std::pair<int64_t, int64_t> speedTest(size_t count);
    static double speedTest();
};

}
#endif // VARIANTTEST_H
