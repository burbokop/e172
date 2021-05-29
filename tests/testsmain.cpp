#include "additionaltest.h"
#include "varianttest.h"
#include <cassert>

int main() {
    e172::AdditionalTest::testAll();
    e172::VariantTest::testAll();
    return 0;
}
