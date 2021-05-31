#ifndef VARIANTTEST_H
#define VARIANTTEST_H

#include <tuple>
#include <stdlib.h>

namespace e172 {


class VariantTest {
public:
    static void testAll();

    static void compareTest0();
    static void compareTest1();

    static void fromJsonTest0();
    static void fromJsonTest1();

    static std::pair<int64_t, int64_t> speedTest(size_t count);
    static double speedTest();
};

}
#endif // VARIANTTEST_H
