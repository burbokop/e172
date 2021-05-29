#include "varianttest.h"

#include <src/variant.h>

#include <src/testing.h>

void e172::VariantTest::testAll() {
    fromJsonTest0();
    fromJsonTest1();
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
    shouldEqual(node1.at("angle"), 0);

}
