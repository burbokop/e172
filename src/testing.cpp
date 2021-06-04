#include "testing.h"

std::list<std::pair<std::string, std::function<void()>>> e172_testing_tests;

int e172::testing::registerTest(const std::string &name, const std::function<void ()> &testFunc) {
    e172_testing_tests.push_back({ name, testFunc });
    return 0;
}

int e172::testing::exec() {
    for(const auto& t : e172_testing_tests) {
        e172::Debug::print("---", t.first, "---");
        t.second();
    }
    return 0;
}
