// Copyright 2023 Borys Boiko

#include "additionalspec.h"

#include "../../src/additional.h"
#include "../../src/math/math.h"
#include "../../src/testing.h"

void e172::tests::AdditionalSpec::parseRadiansTest()
{
    e172_shouldEqual(Additional::parseRadians("Pi/2"), Math::Pi / 2);
    e172_shouldEqual(Additional::parseRadians("Pi / 2"), Math::Pi / 2);
    e172_shouldEqual(Additional::parseRadians("-Pi / 2"), -Math::Pi / 2);
    e172_shouldEqual(Additional::parseRadians("Pi"), Math::Pi);
    e172_shouldEqual(Additional::parseRadians("-Pi"), -Math::Pi);
    e172_shouldEqual(Additional::parseRadians("Pi * 2"), Math::Pi * 2);
    e172_shouldEqual(Additional::parseRadians("-Pi * 2"), -Math::Pi * 2);
    e172_shouldEqual(Additional::parseRadians("2Pi"), Math::Pi * 2);
    e172_shouldEqual(Additional::parseRadians("-2Pi"), -Math::Pi * 2);
    e172_shouldEqual(Additional::parseRadians("-2"), -2);
    e172_shouldEqual(Additional::parseRadians("2"), 2);
    e172_shouldEqual(Additional::parseRadians("0"), 0);
    e172_shouldEqual(Additional::parseRadians(""), None);
}
