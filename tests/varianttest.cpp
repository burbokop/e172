#include "varianttest.h"

#include <src/variant.h>

#include <src/testing.h>

#include <src/time/elapsedtimer.h>

void e172::VariantTest::testAll() {
    Debug::print("benchmark: variant call time / int call time =", speedTest());
    compareTest0();
    compareTest1();
    fromJsonTest0();
    fromJsonTest1();
}

int e172_Variant_ts_d;

void e172_Variant_ts_foo(const e172::Variant &value) {
    e172_Variant_ts_d = value.toInt();
}

void e172_Variant_ts_bar(int value) {
    e172_Variant_ts_d = value;
}

std::pair<int64_t, int64_t> e172::VariantTest::speedTest(size_t count) {
    ElapsedTimer t;
    for(size_t i = 0; i < count; ++i) {
        e172_Variant_ts_foo(i);
    }
    const auto t0 = t.elapsed();
    t.reset();
    for(size_t i = 0; i < count; ++i) {
        e172_Variant_ts_bar(i);
    }
    const auto t1 = t.elapsed();
    return { t0, t1 };
}


double e172::VariantTest::speedTest() {
    size_t c = 1;
    while (true) {
        const auto result = speedTest(c *= 2);
        if(result.first != 0 && result.second != 0) {
            return double(result.first) / double(result.second);
        }
    }
}


void e172::VariantTest::compareTest0() {
    const auto v0 = e172::Variant("123");
    shouldEqual(v0.typeName(), Type<std::string>().name())
    shouldEqual(v0.isNumber(), true)
    const auto v1 = e172::Variant(123);
    shouldEqual(v1.typeName(), "int")
    shouldEqual(v1.isNumber(), true)

    shouldEqual(v0, v1)
    shouldEqual(Variant::typeSafeCompare(v0, v1), false)
}

void e172::VariantTest::compareTest1() {
    const auto v0 = e172::Variant(123.4);
    shouldEqual(v0.typeName(), "double")
    shouldEqual(v0.isNumber(), true)
    const auto v1 = e172::Variant(123.4);
    shouldEqual(v1.typeName(), "double")
    shouldEqual(v1.isNumber(), true)

    shouldEqual(v0, v1)
    shouldEqual(Variant::typeSafeCompare(v0, v1), true)
}


void e172::VariantTest::fromJsonTest1() {
    const auto vec = Variant::fromJson("[\"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\"]").toVector();
    shouldEqual(vec[0], "BuyWareTask>237 scrap");
    shouldEqual(vec[1], "BuyWareTask>237 scrap");
    shouldEqual(vec[2], "BuyWareTask>237 scrap");
}

void e172::VariantTest::fromJsonTest0() {
    const auto map = Variant::fromJson(" \
    { \
        \"id\": \"st2\", \
        \"class\": \"Station\", \
        \"sprite\": \"./st2.png\", \
        \"health\": 340, \
        \"explosive\": 114, \
        \"mass\": 64, \
        \"capabilities\": [ \
            { \
                \"id\": \"docker\", \
                \"class\": \"Docker\", \
                \"nodes\": [ \
                    { \
                        \"offset\": { \
                            \"x\": -50, \
                            \"y\": 0 \
                        }, \
                        \"angle\": \"Pi\" \
                    }, \
                    { \
                        \"offset\": { \
                            \"x\": 50, \
                            \"y\": 0 \
                        }, \
                        \"angle\": \"0\" \
                    } \
                ] \
            } \
        ] \
    } \
    ").toMap();

    shouldEqual(map.at("id"), "st2")
    shouldEqual(map.at("class"), "Station")
    shouldEqual(map.at("sprite"), "./st2.png")
    shouldEqual(map.at("health"), 340)
    shouldEqual(map.at("explosive"), 114)
    shouldEqual(map.at("mass"), 64)
    const auto capabilitiesList = map.at("capabilities").toList();
    shouldEqual(capabilitiesList.size(), 1)

    const auto capabilities = map.at("capabilities").toVector();
    shouldEqual(capabilities.size(), 1)
    const auto cap = capabilities[0].toMap();

    shouldEqual(cap.at("id"), "docker");
    shouldEqual(cap.at("class"), "Docker");

    const auto nodes = cap.at("nodes").toVector();
    shouldEqual(nodes.size(), 2);

    const auto node0 = nodes[0].toMap();
    const auto node0Offset = node0.at("offset").toMap();
    shouldEqual(node0Offset.at("x"), -50);
    shouldEqual(node0Offset.at("y"), 0);
    shouldEqual(node0.at("angle"), "Pi");

    const auto node1 = nodes[0].toMap();
    const auto node1Offset = node1.at("offset").toMap();
    shouldEqual(node1Offset.at("x"), 50);
    shouldEqual(node1Offset.at("y"), 0);
    shouldEqual(node1.at("angle"), "Pi");

}

