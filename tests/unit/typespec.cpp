#include "typespec.h"

class Clazz {};

struct Structure {};

namespace e172::tests {

class NClazz {};

struct NStructure {};

void TypeSpec::simpleTypeTest()
{
    e172_shouldEqual(Type<int>::name(), "int");
    e172_shouldEqual(Type<double>::name(), "double");
}

void TypeSpec::complexTypeTest()
{
    e172_shouldEqual(Type<Clazz>::name(), "Clazz");
    e172_shouldEqual(Type<Structure>::name(), "Structure");
}

void TypeSpec::namespacedTypeTest()
{
    e172_shouldEqual(Type<NClazz>::name(), "e172::tests::NClazz");
    e172_shouldEqual(Type<NStructure>::name(), "e172::tests::NStructure");
}

}
