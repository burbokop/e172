#pragma once

#include <src/testing.h>

namespace e172::tests {

class PackageSpec
{
    static void writePackageTest() e172_test(PackageSpec, writePackageTest);
    static void readPackageTest() e172_test(PackageSpec, readPackageTest);
    static void readPackageTestFail() e172_test(PackageSpec, readPackageTestFail);
    static void readWritePackageTest() e172_test(PackageSpec, readWritePackageTest);
};

} // namespace e172::tests
