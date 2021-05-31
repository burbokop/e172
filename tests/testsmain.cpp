#include "additionaltest.h"
#include "priorityproceduretest.h"
#include "varianttest.h"

int main() {
    e172::AdditionalTest::testAll();
    e172::VariantTest::testAll();
    e172::PriorityProcedureTest::testAll();
    return 0;
}
