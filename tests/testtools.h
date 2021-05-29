#ifndef TESTTOOLS_H
#define TESTTOOLS_H

#include <iostream>

#define shouldEqual(actual, expected) \
    if(actual != expected) { \
        std::cerr << actual << " is not " << expected << " here " << __FILE__ << ":" << __LINE__ << ": " << __ASSERT_FUNCTION << std::endl; \
        exit(1); \
    }

#endif // TESTTOOLS_H
