// Copyright 2023 Borys Boiko

#include "variantbenches.h"

#include "../src/time/elapsedtimer.h"
#include "../src/variant.h"
#include <utility>

volatile int e172_Variant_ts_d;

void e172_Variant_ts_foo(const e172::Variant &value)
{
    e172_Variant_ts_d = value.toInt();
}

void e172_Variant_ts_bar(int value)
{
    e172_Variant_ts_d = value;
}

std::pair<int64_t, int64_t> e172::benches::VariantBenches::speedTest(size_t count)
{
    ElapsedTimer t;
    for (int i = 0; i < count; ++i) {
        e172_Variant_ts_foo(i);
    }
    const auto t0 = t.elapsed();
    t.reset();
    for (int i = 0; i < count; ++i) {
        e172_Variant_ts_bar(i);
    }
    const auto t1 = t.elapsed();
    return { t0, t1 };
}

double e172::benches::VariantBenches::speedTest()
{
    size_t c = 1;
    ElapsedTimer timeout(100000);
    while (true) {
        const auto result = speedTest(c *= 2);
        if (result.first != 0 && result.second != 0) {
            return static_cast<double>(result.first) / static_cast<double>(result.second);
        }
        if (timeout.check()) {
            throw std::runtime_error("can not measure Variant speed: timeout");
        }
    }
}

void e172::benches::VariantBenches::banchmark()
{
    double sum = 0;
    for (size_t i = 0; i < 100; ++i) {
        const auto s = speedTest();
        Debug::print("[", i, "%] e172::VariantTest::speedTest:", s);
        sum += s;
    }
    Debug::print("e172::VariantTest::speedTest (average):", static_cast<std::size_t>(sum / 100.));
}
