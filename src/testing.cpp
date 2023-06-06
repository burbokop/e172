#include "testing.h"

#include "consolecolor.h"
#include <map>

namespace e172 {
namespace testing {

struct test {
    std::string name;
    std::function<void()> testFunc;
};

struct spec {
    std::list<test> tests;
};

std::map<std::string, spec> specs;

}
}

int e172::testing::registerTest(const std::string &name, const std::string &spec, const std::function<void ()> &testFunc) {
    specs[spec].tests.push_back({ name, testFunc });
    return 0;
}

int e172::testing::exec() {
    for(const auto& s : specs) {
        e172::Debug::print("----", s.first, "----");
        for(const auto& t : s.second.tests) {
            e172::Debug::print("test: ", t.name);
            t.testFunc();
            e172::Debug::print("      ", t.name, Green.wrap("OK"));
        }
    }
    return 0;
}

