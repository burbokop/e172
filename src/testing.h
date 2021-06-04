#ifndef TESTING_H
#define TESTING_H

#include "debug.h"
#include <cassert>

#define e172_shouldEqual(actual, expected) \
    if(actual != expected) { \
        e172::Debug::fatal(actual, "is not", expected, "here", e172::Debug::codeLocation(__FILE__, __LINE__), __ASSERT_FUNCTION); \
    }

namespace e172 {
namespace testing {

int registerTest(const std::string& name, const std::function<void()> &testFunc);
int exec();

}
}

#define e172_test(NAME) static int inline __ ## NAME ## _test_registration = e172::testing::registerTest(#NAME, &NAME);

#endif // TESTING_H
