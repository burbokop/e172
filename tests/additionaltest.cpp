#include "additionaltest.h"

#include <src/additional.h>
#include <cassert>
#include <src/math/math.h>
#include <src/testing.h>

void e172::AdditionalTest::testAll() {
    parseRadiansTest();
}



void e172::AdditionalTest::parseRadiansTest() {
    shouldEqual(Additional::parseRadians("Pi/2"), Math::Pi / 2);
    shouldEqual(Additional::parseRadians("Pi / 2"), Math::Pi / 2);
    shouldEqual(Additional::parseRadians("-Pi / 2"), -Math::Pi / 2);
    shouldEqual(Additional::parseRadians("Pi"), Math::Pi);
    shouldEqual(Additional::parseRadians("-Pi"), -Math::Pi);
    shouldEqual(Additional::parseRadians("Pi * 2"), Math::Pi * 2);
    shouldEqual(Additional::parseRadians("-Pi * 2"), -Math::Pi * 2);
    shouldEqual(Additional::parseRadians("2Pi"), Math::Pi * 2);
    shouldEqual(Additional::parseRadians("-2Pi"), -Math::Pi * 2);
    shouldEqual(Additional::parseRadians("-2"), -2);
    shouldEqual(Additional::parseRadians("2"), 2);
    shouldEqual(Additional::parseRadians("0"), 0);
    shouldEqual(Additional::parseRadians(""), None);
}
