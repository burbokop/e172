#ifndef TESTING_H
#define TESTING_H

#include "debug.h"
#include <cassert>
#include "type.h"

#define e172_shouldEqual(actual, expected) \
    if(actual != expected) { \
        e172::Debug::fatal(actual, "is not", expected, "here", e172::Debug::codeLocation(__FILE__, __LINE__), __ASSERT_FUNCTION); \
    }

#define e172_shouldBeDefined(option) \
    if(option.isEmpty()) { \
        e172::Debug::fatal("option is not defined here", e172::Debug::codeLocation(__FILE__, __LINE__), __ASSERT_FUNCTION); \
    }


namespace e172 {
namespace testing {

int registerTest(const std::string& name, const std::string& spec, const std::function<void()> &testFunc);
int exec();

template<typename T>
class class_registerer {
    static const inline int static_call = registerTestClass(Type<T>().name());
public:
    constexpr class_registerer() { (void)static_call; }
};


}
}

#define e172_test(SPEC, TEST) static int inline __ ## TEST ## _test_registration = e172::testing::registerTest(#TEST, #SPEC, &TEST);

#define e172_register_class(TAG) auto __ ## TAG ## _class_registration() { \
    static constexpr e172::testing::class_registerer<std::remove_pointer<decltype (this)>::type> result; \
    return result; \
}


#endif // TESTING_H
