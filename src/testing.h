#ifndef TESTING_H
#define TESTING_H

#include "debug.h"

#define shouldEqual(actual, expected) \
    if(actual != expected) { \
        e172::Debug::fatal(actual, "is not", expected, "here", e172::Debug::codeLocation(__FILE__, __LINE__), __ASSERT_FUNCTION); \
    }

#endif // TESTING_H
