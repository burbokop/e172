// Copyright 2023 Borys Boiko

#pragma once

#include "../traits.h"
#include "either.h"
#include <functional>
#include <list>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace e172 {

/**
 * @brief The Flag class - used to declare command line mandatory flag
 */
struct Flag
{
    /**
     * @brief shortName
     * @example `a` in command line looks as `-a`
     */
    std::string shortName;
    /**
     * @brief longName
     * @example `a` in command line looks as `--a`
     */
    std::string longName;
    std::string description;
};

/**
 * @brief The OptFlag class - used to declare command line optional flag
 */
template<typename T>
struct OptFlag
{
    /**
     * @brief shortName
     * @example `a` in command line looks as `-a`
     */
    std::string shortName;
    /**
     * @brief longName
     * @example `a` in command line looks as `--a`
     */
    std::string longName;
    std::string description;
    T defaultVal;
};

enum class FlagParseError {
    MandatoryFlagNotFound,
    ValueAfterFlagMissing,
    NumberParsingFailed,
    EnumValueNotFound
};

inline std::ostream &operator<<(std::ostream &stream, FlagParseError err)
{
    switch (err) {
    case FlagParseError::MandatoryFlagNotFound:
        return stream << "FlagNotFound";
    case FlagParseError::ValueAfterFlagMissing:
        return stream << "ValueAfterFlagMissing";
    case FlagParseError::NumberParsingFailed:
        return stream << "NumberParsingFailed";
    case FlagParseError::EnumValueNotFound:
        return stream << "EnumValueNotFound";
    }
    return stream << "UnknownError";
}

struct RawFlagValue
{
    std::string str;
};

/**
 * @brief ParseFlag - requires flag parse operator (operator>>(RawFlagValue&, TypeTag<T>))
 * Define this operator if you want your custom type to be parsed from flag value
 */
template<typename T>
concept ParseFlag = requires(RawFlagValue &flag, TypeTag<T> tag) {
    {
        flag >> tag
    } -> std::convertible_to<Either<FlagParseError, T>>;
};

template<std::integral T>
Either<FlagParseError, T> operator>>(RawFlagValue &raw, TypeTag<T>)
{
    try {
        return Right<T>(static_cast<T>(std::stoll(raw.str)));
    } catch (...) {
        return Left(FlagParseError::NumberParsingFailed);
    }
}

template<std::floating_point T>
Either<FlagParseError, T> operator>>(RawFlagValue &raw, TypeTag<T>)
{
    try {
        return Right<T>(std::stod(raw.str));
    } catch (...) {
        return Left(FlagParseError::NumberParsingFailed);
    }
}

template<ParseFlag T>
Either<FlagParseError, std::optional<T>> operator>>(RawFlagValue &raw, TypeTag<std::optional<T>>)
{
    return (raw >> TypeTag<T>{}).template map<std::optional<T>>([](const auto &v) { return v; });
}

template<std::constructible_from<std::string> T>
Either<FlagParseError, T> operator>>(RawFlagValue &raw, TypeTag<T>)
{
    return Right(raw.str);
}

class FlagParser
{
public:
    FlagParser() = delete;
    FlagParser(const FlagParser &) = delete;
    FlagParser(FlagParser &&) = delete;

    const auto &parsedFlags() const { return m_parsedFlags; }
    std::optional<std::string> appname() const;

    static std::vector<std::string> coverArgs(int argc, const char **argv);

    template<typename T>
    using DeclareFlagsFunc = std::function<T(FlagParser &)>;
    using HandleErrorFunc = std::function<void(const FlagParser &)>;
    using HandleHelpFunc = std::function<void(const FlagParser &)>;
    using HandleVersionFunc = std::function<void(const FlagParser &)>;

    template<typename T>
    static std::optional<T> parse(const std::vector<std::string> &args,
                                  const DeclareFlagsFunc<T> &declareFlagsFunc,
                                  const HandleErrorFunc &handleErrorFunc,
                                  const HandleHelpFunc &handleHelpFunc,
                                  const HandleVersionFunc &handleVersionFunc)
    {
        FlagParser p(args);
        const auto &&result = declareFlagsFunc(p);
        if (p.finish(handleErrorFunc, handleHelpFunc, handleVersionFunc)) {
            return result;
        } else {
            return std::nullopt;
        }
    }

    template<typename T>
    static std::optional<T> parse(int argc,
                                  const char **argv,
                                  const DeclareFlagsFunc<T> &declareFlagsFunc,
                                  const HandleErrorFunc &handleErrorFunc,
                                  const HandleHelpFunc &handleHelpFunc,
                                  const HandleVersionFunc &handleVersionFunc)
    {
        return parse(coverArgs(argc, argv),
                     declareFlagsFunc,
                     handleErrorFunc,
                     handleHelpFunc,
                     handleVersionFunc);
    }

    template<typename T>
    T flag(Flag &&f)
        requires ParseFlag<T> || std::is_same<T, bool>::value
    {
        if constexpr (std::is_same<T, bool>::value) {
            m_parsedFlags.push_back({.shortName = f.shortName,
                                     .longName = f.longName,
                                     .description = f.description,
                                     .isBool = true,
                                     .isOptional = false,
                                     .err = std::nullopt,
                                     .raw = {},
                                     .isLongProvided = false});
            return popBoolArg(f.shortName, f.longName);
        } else {
            const auto &val = popArgAfterAs<T>(f.shortName, f.longName);
            m_parsedFlags.push_back({.shortName = f.shortName,
                                     .longName = f.longName,
                                     .description = f.description,
                                     .isBool = false,
                                     .isOptional = false,
                                     .err = val.value.left().option(),
                                     .raw = val.raw,
                                     .isLongProvided = val.isLongProvided});
            return val.value.getOr();
        }
    }

    template<ParseFlag T>
    T flag(OptFlag<T> &&f)
    {
        const auto &val = popArgAfterAs<T>(f.shortName, f.longName);
        std::optional<FlagParseError> err;
        bool notFound = false;
        if (!val.value) {
            err = val.value.left().value();
            if (*err == FlagParseError::MandatoryFlagNotFound) {
                err = std::nullopt;
                notFound = true;
            }
        }
        m_parsedFlags.push_back({.shortName = f.shortName,
                                 .longName = f.longName,
                                 .description = f.description,
                                 .isBool = false,
                                 .isOptional = false,
                                 .err = err,
                                 .raw = val.raw,
                                 .isLongProvided = val.isLongProvided});

        return notFound ? f.defaultVal : val.value.getOr();
    }

    void displayHelp(std::ostream &stream) const;
    void displayErr(std::ostream &stream) const;
    void displayVersion(std::ostream &stream, const std::string &version) const;

private:
    struct ParsedFlag
    {
        /// Desctiption fields
        std::string shortName;
        std::string longName;
        std::string description;
        bool isBool;
        bool isOptional;
        /// Parsing result fileds
        std::optional<FlagParseError> err;
        std::string raw;
        bool isLongProvided;
    };

    template<ParseFlag T>
    struct ArgAfterAsResult
    {
        Either<FlagParseError, T> value;
        std::string raw;
        bool isLongProvided;
    };

    FlagParser(const std::vector<std::string> &args)
        : m_arguments(args)
    {}

    bool finish(const HandleErrorFunc &handleErrorFunc,
                const HandleHelpFunc &handleHelpFunc,
                const HandleVersionFunc &handleVersionFunc);

    bool hasErrors() const;

    bool popBoolArg(const std::string &arg);
    bool popBoolArg(const std::string &shortName, const std::string &longName);

    Either<FlagParseError, std::string> popArgAfter(const std::string &arg);

    /**
     * @brief nextArg
     * @param shortName
     * @param longName
     * @return pair { value or error, is long provided }
     */
    std::pair<Either<FlagParseError, std::string>, bool> popArgAfter(const std::string &shortName,
                                                                     const std::string &longName);

    template<ParseFlag T>
    ArgAfterAsResult<T> popArgAfterAs(const std::string &shortName, const std::string &longName)
    {
        auto val = popArgAfter(shortName, longName);
        if (val.first) {
            RawFlagValue raw{.str = val.first.right().value()};
            return {.value = raw >> TypeTag<T>{},
                    .raw = std::move(val.first.right().value()),
                    .isLongProvided = val.second};
        } else {
            return {val.first.left(), {}, val.second};
        }
    }
private:
    std::list<ParsedFlag> m_parsedFlags;
    std::vector<std::string> m_arguments;
};

} // namespace e172
