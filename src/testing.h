// Copyright 2023 Borys Boiko

#pragma once

#include "debug.h"
#include "type.h"
#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <string>

namespace e172::testing {

struct Test {
    std::string name;
    std::function<void()> testFunc;
};

struct Spec {
    std::list<Test> tests;

    std::optional<const Test> findTest(const std::string &str) const
    {
        for (const auto &t : tests) {
            if (t.name == str) {
                return t;
            }
        }
        return std::nullopt;
    }
};

class Registry {
public:
    friend int exec(int argc, const char **argv);

    static int registerTest(const std::string& name, const std::string& spec, const std::function<void()>& func);
private:
    static inline std::map<std::string, Spec> s_specs;
};

int exec(int argc, const char **argv);

} // namespace e172::testing

#if defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER)
#define e172_pretty_function __FUNCSIG__
#else
#define e172_pretty_function __PRETTY_FUNCTION__
#endif

#define e172_initializerList(type, ...) \
type \
{ \
        __VA_ARGS__ \
}

#define e172_shouldEqual(actual, expected) \
if ((actual) != (expected)) { \
        e172::Debug::fatal(e172::Debug::codeLocation(__FILE__, __LINE__), \
                           "Assertion failed:", \
                           actual, \
                           "is not", \
                           expected, \
                           "in test function:", \
                           e172_pretty_function); \
}

#define e172_shouldNotEqual(actual, expected) \
    if ((actual) == (expected)) { \
        e172::Debug::fatal(e172::Debug::codeLocation(__FILE__, __LINE__), \
                           "Assertion failed:", \
                           actual, \
                           "should not be equal to", \
                           expected, \
                           "in test function:", \
                           e172_pretty_function); \
    }

#define e172_shouldBeDefined(option) \
if ((option).isEmpty()) { \
        e172::Debug::fatal(e172::Debug::codeLocation(__FILE__, __LINE__), \
                           "Option is not defined in test function:", \
                           e172_pretty_function); \
}

#define e172_test(SPEC, TEST) \
    ; \
    static int inline __##TEST##_test_registration = e172::testing::Registry::registerTest(#TEST, \
                                                                                 #SPEC, \
                                                                                 &TEST)
