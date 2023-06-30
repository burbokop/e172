// Copyright 2023 Borys Boiko

#include "variantspec.h"

#include "../../src/additional.h"
#include "../../src/testing.h"
#include "../../src/variant.h"
#include <iostream>
#include <string>

void e172::tests::VariantSpec::rttiTableTest() {
    const auto int0 = VariantRTTITable<int>::object();
    const auto int1 = VariantRTTITable<int>::object();
    e172_shouldEqual(int0, int1)

    const auto str0 = VariantRTTITable<std::string>::object();
    const auto str1 = VariantRTTITable<std::string>::object();
    e172_shouldEqual(str0, str1)
}

void e172::tests::VariantSpec::compareTest0()
{
    e172_shouldEqual(Variant::containsNumber("123"), true)

    const auto v0 = Variant("123");
    e172_shouldEqual(v0.typeName(), Type<std::string>().name())
    e172_shouldEqual(v0.containsType<std::string>(), true)
    e172_shouldEqual(v0.value<std::string>(), "123")
    e172_shouldEqual(v0.isNumber(), true)
    const auto v1 = Variant(123);
    e172_shouldEqual(v1.typeName(), Type<int>().name())
    e172_shouldEqual(v1.isNumber(), true)

    e172_shouldEqual(v0, v1)
    e172_shouldEqual(Variant::typeSafeCompare(v0, v1), false)
}

void e172::tests::VariantSpec::compareTest1()
{
    const auto v0 = e172::Variant(123.4);
    e172_shouldEqual(v0.typeName(), "double")
    e172_shouldEqual(v0.isNumber(), true)
    const auto v1 = e172::Variant(123.4);
    e172_shouldEqual(v1.typeName(), "double")
    e172_shouldEqual(v1.isNumber(), true)

    e172_shouldEqual(v0, v1)
    e172_shouldEqual(Variant::typeSafeCompare(v0, v1), true)
}

void e172::tests::VariantSpec::compareEnumTest()
{
    enum TestEnum : std::uint32_t {
        Val0 = 0,
        Val1 = 0x1000,
    };

    e172_shouldEqual(e172::Variant(Val0), e172::Variant(Val0));
    e172_shouldEqual(e172::Variant(Val1), e172::Variant(Val1));
    e172_shouldNotEqual(e172::Variant(Val0), e172::Variant(Val1));
    e172_shouldEqual(e172::Variant(Val0) < e172::Variant(Val0), false);
    e172_shouldEqual(e172::Variant(Val0) < e172::Variant(Val1), true);

    e172_shouldEqual(e172::Variant(Val0), 0);
    e172_shouldEqual(e172::Variant(Val1), 0x1000);

    enum class TestEnumClass : std::underlying_type<TestEnum>::type {
        Val0 = TestEnum::Val1,
        Val1,
    };

    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val0)),
                     e172::Variant(TestEnum(TestEnumClass::Val0)));
    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val1)),
                     e172::Variant(TestEnum(TestEnumClass::Val1)));
    e172_shouldNotEqual(e172::Variant(TestEnum(TestEnumClass::Val0)),
                        e172::Variant(TestEnum(TestEnumClass::Val1)));
    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val0))
                         < e172::Variant(TestEnum(TestEnumClass::Val0)),
                     false);
    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val0))
                         < e172::Variant(TestEnum(TestEnumClass::Val1)),
                     true);

    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val0)), 0x1000);
    e172_shouldEqual(e172::Variant(TestEnum(TestEnumClass::Val1)), 0x1001);
}

void e172::tests::VariantSpec::fromJsonTest1()
{
    const auto vec
        = Variant::fromJson(
              "[\"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\", \"BuyWareTask>237 scrap\"]")
              .toVector();
    e172_shouldEqual(vec[0], "BuyWareTask>237 scrap");
    e172_shouldEqual(vec[1], "BuyWareTask>237 scrap");
    e172_shouldEqual(vec[2], "BuyWareTask>237 scrap");
}

void e172::tests::VariantSpec::fromJsonTest0()
{
    const auto jsonStr = Additional::stripMargins(R"(
        |{
        |    "id": "st2",
        |    "class": "Station",
        |    "sprite": "./st2.png",
        |    "health": 340,
        |    "explosive": 114,
        |    "mass": 64,
        |    "capabilities": [
        |        {
        |            "id": "docker",
        |            "class": "Docker",
        |            "nodes": [
        |                {
        |                    "offset": {
        |                        "x": -50,
        |                        "y": 0
        |                    },
        |                    "angle": "Pi"
        |                },
        |                {
        |                    "offset": {
        |                        "x": 50,
        |                        "y": 0
        |                    },
        |                    "angle": "0"
        |                }
        |            ]
        |        }
        |    ]
        |})");

    std::cout << "jsonStr: <" << jsonStr << ">" << std::endl;

    const auto map = Variant::fromJson(jsonStr).toMap();

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
