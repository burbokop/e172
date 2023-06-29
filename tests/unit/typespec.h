// Copyright 2023 Borys Boiko

#pragma once

#include "../../src/testing.h"

namespace e172::tests {

class TypeSpec
{
    static void simpleTypeTest() e172_test(TypeSpec, simpleTypeTest);
    static void complexTypeTest() e172_test(TypeSpec, complexTypeTest);
    static void namespacedTypeTest() e172_test(TypeSpec, namespacedTypeTest);
};

}
