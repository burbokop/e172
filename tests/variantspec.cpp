#include "variantspec.h"

#include <src/variant.h>

#include <src/testing.h>

#include <src/time/elapsedtimer.h>


int e172_Variant_ts_d;

void e172_Variant_ts_foo(const e172::Variant &value) {
    e172_Variant_ts_d = value.toInt();
}

void e172_Variant_ts_bar(int value) {
    e172_Variant_ts_d = value;
}

std::pair<int64_t, int64_t> e172::VariantSpec::speedTest(size_t count) {
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


double e172::VariantSpec::speedTest() {
    size_t c = 1;
    while (true) {
        const auto result = speedTest(c *= 2);
        if(result.first != 0 && result.second != 0) {
            return double(result.first) / double(result.second);
        }
    }
}


void e172::VariantSpec::compareTest0() {
    const auto v0 = e172::Variant("123");
    e172_shouldEqual(v0.typeName(), Type<std::string>().name())
    e172_shouldEqual(v0.isNumber(), true)
    const auto v1 = e172::Variant(123);
    e172_shouldEqual(v1.typeName(), "int")
    e172_shouldEqual(v1.isNumber(), true)

    e172_shouldEqual(v0, v1)
    e172_shouldEqual(Variant::typeSafeCompare(v0, v1), false)
}

void e172::VariantSpec::compareTest1() {
    const auto v0 = e172::Variant(123.4);
    e172_shouldEqual(v0.typeName(), "double")
    e172_shouldEqual(v0.isNumber(), true)
    const auto v1 = e172::Variant(123.4);
    e172_shouldEqual(v1.typeName(), "double")
    e172_shouldEqual(v1.isNumber(), true)

    e172_shouldEqual(v0, v1)
    e172_shouldEqual(Variant::typeSafeCompare(v0, v1), true)
}


void e172::VariantSpec::fromJsonTest1() {
    const auto vec = Variant::fromJson("[\"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\"]").toVector();
    e172_shouldEqual(vec[0], "BuyWareTask>237 scrap");
    e172_shouldEqual(vec[1], "BuyWareTask>237 scrap");
    e172_shouldEqual(vec[2], "BuyWareTask>237 scrap");
}

void e172::VariantSpec::banchmark() {
    size_t sum = 0;
    for(size_t i = 0; i < 100; ++i) {
        const auto s = speedTest();
        Debug::print("[", i, "%] e172::VariantTest::speedTest:", s);
        sum += s;
    }
    Debug::print("e172::VariantTest::speedTest (average):", sum / 100);
}

void e172::VariantSpec::fromJsonTest0() {
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

    e172_shouldEqual(map.at("id"), "st2")
    e172_shouldEqual(map.at("class"), "Station")
    e172_shouldEqual(map.at("sprite"), "./st2.png")
    e172_shouldEqual(map.at("health"), 340)
    e172_shouldEqual(map.at("explosive"), 114)
    e172_shouldEqual(map.at("mass"), 64)
    const auto capabilitiesList = map.at("capabilities").toList();
    e172_shouldEqual(capabilitiesList.size(), 1)

    const auto capabilities = map.at("capabilities").toVector();
    e172_shouldEqual(capabilities.size(), 1)
    const auto cap = capabilities[0].toMap();

    e172_shouldEqual(cap.at("id"), "docker");
    e172_shouldEqual(cap.at("class"), "Docker");

    const auto nodes = cap.at("nodes").toVector();
    e172_shouldEqual(nodes.size(), 2);

    const auto node0 = nodes[0].toMap();
    const auto node0Offset = node0.at("offset").toMap();
    e172_shouldEqual(node0Offset.at("x"), -50);
    e172_shouldEqual(node0Offset.at("y"), 0);
    e172_shouldEqual(node0.at("angle"), "Pi");

    const auto node1 = nodes[0].toMap();
    const auto node1Offset = node1.at("offset").toMap();
    e172_shouldEqual(node1Offset.at("x"), 50);
    e172_shouldEqual(node1Offset.at("y"), 0);
    e172_shouldEqual(node1.at("angle"), "Pi");

}

