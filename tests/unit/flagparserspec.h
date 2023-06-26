#pragma once

#include <src/testing.h>

namespace e172::tests {

class FlagParserSpec
{
    static void parseBoolTest() e172_test(FlagParserSpec, parseBoolTest);
    static void parsePrimitiveTest() e172_test(FlagParserSpec, parsePrimitiveTest);
    static void parsePrimitiveDefaultTest() e172_test(FlagParserSpec, parsePrimitiveDefaultTest);
    static void parsePrimitiveDefaultOptTest()
        e172_test(FlagParserSpec, parsePrimitiveDefaultOptTest);
    static void parseUserTypeTest() e172_test(FlagParserSpec, parseUserTypeTest);
    static void parseStringTest() e172_test(FlagParserSpec, parseStringTest);
    static void unrecognizedFlagTest() e172_test(FlagParserSpec, unrecognizedFlagTest);
};

} // namespace e172::tests
