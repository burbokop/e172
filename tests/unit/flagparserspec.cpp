#include "flagparserspec.h"

#include <iostream>
#include <src/utility/flagparser.h>

namespace e172::tests {

void FlagParserSpec::parseBoolTest()
{
    struct Flags
    {
        bool verbose;
    };

    const auto &&rules = [](FlagParser &parser) {
        return Flags{.verbose = parser.flag<bool>(
                         Flag{.shortName = "v", .longName = "verbose", .description = "number"})};
    };

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "--verbose"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.verbose, true);
    }

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "-v"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.verbose, true);
    }

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.verbose, false);
    }
}

void FlagParserSpec::parsePrimitiveTest()
{
    struct Flags
    {
        std::uint32_t num;
    };

    const auto &&rules = [](FlagParser &parser) {
        return Flags{.num = parser.flag<std::uint32_t>(
                         Flag{.shortName = "n", .longName = "num", .description = "number"})};
    };

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "-n", "12"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.num, 12);
    }

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "--num", "5"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.num, 5);
    }

    {
        bool errored = false;
        const auto t = FlagParser::parse<Flags>(
            std::vector<std::string>{"app", "--num"},
            rules,
            [&errored](auto &) { errored = true; },
            nullptr,
            nullptr);

        e172_shouldEqual(t.has_value(), false);
        e172_shouldEqual(errored, true);
    }
}

void FlagParserSpec::parsePrimitiveDefaultTest()
{
    struct Flags
    {
        std::uint32_t num;
    };

    const auto &&rules = [](FlagParser &parser) {
        return Flags{.num = parser.flag(OptFlag<std::uint32_t>{.shortName = "n",
                                                               .longName = "num",
                                                               .description = "number",
                                                               .defaultVal = 2})};
    };

    {
        const auto t = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app"},
                           rules,
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

        e172_shouldEqual(t.num, 2);
    }
}

void FlagParserSpec::parsePrimitiveDefaultOptTest()
{
    struct Flags
    {
        std::optional<std::uint32_t> num;
    };

    const auto flags
        = FlagParser::parse<Flags>(
              std::vector<std::string>{"app"},
              [](FlagParser &parser) {
                  return Flags{.num = parser.flag(
                                   OptFlag<std::optional<std::uint32_t>>{.shortName = "n",
                                                                         .longName = "num",
                                                                         .description = "number"})};
              },
              [](const auto &p) {
                  std::ostringstream ss;
                  p.displayErr(ss);
                  e172_shouldNoBeExecutedBut(ss.str());
              },
              nullptr,
              nullptr)
              .value();

    e172_shouldEqual(flags.num.has_value(), false);
}

enum class TestEnum { Val0, Val1, Val2 };

std::ostream &operator<<(std::ostream &stream, TestEnum e)
{
    switch (e) {
    case TestEnum::Val0:
        return stream << "Val0";
    case TestEnum::Val1:
        return stream << "Val1";
    case TestEnum::Val2:
        return stream << "Val2";
    }
}

/**
 * @brief operator >> - custom type flag parsing operator
 * @param flag
 * @return 
 */
Either<FlagParseError, TestEnum> operator>>(RawFlagValue &flag, TypeTag<TestEnum>)
{
    if (flag.str == "val0") {
        return Right(TestEnum::Val0);
    } else if (flag.str == "val1") {
        return Right(TestEnum::Val1);
    } else if (flag.str == "val2") {
        return Right(TestEnum::Val2);
    } else {
        return Left(FlagParseError::EnumValueNotFound);
    }
}

void FlagParserSpec::parseUserTypeTest()
{
    struct Flags
    {
        TestEnum num;
    };

    const auto flags = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "-n", "val1"},
                           [](FlagParser &parser) {
                               return Flags{
                                   .num = parser.flag<TestEnum>(Flag{.shortName = "n",
                                                                     .longName = "num",
                                                                     .description = "test enum"})};
                           },
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

    e172_shouldEqual(flags.num, TestEnum::Val1);
}
void FlagParserSpec::parseStringTest()
{
    struct Flags
    {
        std::string val;
    };

    const auto flags = FlagParser::parse<Flags>(
                           std::vector<std::string>{"app", "-v", "gogadoda"},
                           [](FlagParser &parser) {
                               return Flags{.val = parser.flag<std::string>(
                                                Flag{.shortName = "v",
                                                     .longName = "val",
                                                     .description = "string value"})};
                           },
                           [](const auto &p) {
                               std::ostringstream ss;
                               p.displayErr(ss);
                               e172_shouldNoBeExecutedBut(ss.str());
                           },
                           nullptr,
                           nullptr)
                           .value();

    e172_shouldEqual(flags.val, "gogadoda");
}

void FlagParserSpec::unrecognizedFlagTest()
{
    struct Flags
    {
        std::string val;
    };

    bool errored = false;
    const auto flags = FlagParser::parse<Flags>(
        std::vector<std::string>{"app", "--ababa"},
        [](FlagParser &parser) {
            return Flags{
                .val = parser.flag<std::string>(
                    Flag{.shortName = "v", .longName = "val", .description = "string value"})};
        },
        [&errored](const auto &p) { errored = true; },
        nullptr,
        nullptr);

    e172_shouldEqual(flags.has_value(), false);
    e172_shouldEqual(errored, true);
}

} // namespace e172::tests
