#include "testing.h"

#include "consolecolor.h"
#include "src/todo.h"
#include <map>
#include <optional>
#include <src/additional.h>

namespace e172 {
namespace testing {

namespace {

struct test {
    std::string name;
    std::function<void()> testFunc;
};

struct spec {
    std::list<test> tests;

    std::optional<const test> findTest(const std::string &str) const
    {
        for (const auto &t : tests) {
            if (t.name == str) {
                return t;
            }
        }
        return std::nullopt;
    }
};

std::map<std::string, spec> specs;

} // namespace

int registerTest(const std::string &name,
                 const std::string &spec,
                 const std::function<void()> &testFunc)
{
    specs[spec].tests.push_back({ name, testFunc });
    return 0;
}

int exec(int argc, const char **argv)
{
    if (argc > 1) {
        const std::string command = argv[1];
        if (command == "enum") {
            for (const auto &s : specs) {
                for (const auto &t : s.second.tests) {
                    e172::Debug::withSepar("").emitMessage(Debug::PrintMessage,
                                                           s.first,
                                                           ":",
                                                           t.name);
                }
            }
            return 0;
        } else if (command == "one") {
            if (argc > 2) {
                const std::string test = argv[2];
                const auto p = Additional::splitIntoPair(test, ':');

                const auto specIt = specs.find(p.first);
                if (specIt != specs.end()) {
                    if (const auto &test = specIt->second.findTest(p.second)) {
                        test->testFunc();
                        return 0;
                    } else {
                        e172::Debug::withSepar("").emitMessage(Debug::WarningMessage,
                                                               "test ",
                                                               p.second,
                                                               " not found");
                        return 3;
                    }
                } else {
                    e172::Debug::withSepar("").emitMessage(Debug::WarningMessage,
                                                           "spec ",
                                                           p.first,
                                                           " not found");
                    return 3;
                }
            } else {
                e172::Debug::withSepar("").emitMessage(Debug::WarningMessage,
                                                       "test id not provided");
                return 2;
            }
        } else if (command == "spec") {
            todo;
        } else {
            e172::Debug::withSepar("").emitMessage(Debug::WarningMessage,
                                                   command,
                                                   ": unknown command");
            return 1;
        }
    }
    for (const auto &s : specs) {
        e172::Debug::print("----", s.first, "----");
        for (const auto &t : s.second.tests) {
            e172::Debug::print("test: ", t.name);
            t.testFunc();
            e172::Debug::print("      ", t.name, Green.wrap("OK"));
        }
    }
    return 0;
}

} // namespace testing
} // namespace e172
