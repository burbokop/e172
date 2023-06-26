// Copyright 2023 Borys Boiko

#include "flagparser.h"

#include "src/consolecolor.h"
#include <algorithm>
#include <filesystem>

namespace e172 {

std::optional<std::string> FlagParser::appname() const
{
    if (m_arguments.size() > 0) {
        return std::filesystem::path(m_arguments.front()).filename().string();
    }
    return std::nullopt;
}

bool e172::FlagParser::popBoolArg(const std::string &arg)
{
    auto it = std::find(m_arguments.begin(), m_arguments.end(), arg);
    if (it != m_arguments.end()) {
        m_arguments.erase(it);
        return true;
    }
    return false;
}

bool FlagParser::popBoolArg(const std::string &shortName, const std::string &longName)
{
    return popBoolArg("-" + shortName) || popBoolArg("--" + longName);
}

Either<FlagParseError, std::string> FlagParser::popArgAfter(const std::string &arg)
{
    auto it = std::find(m_arguments.begin(), m_arguments.end(), arg);
    if (it != m_arguments.end()) {
        it++;
        if (it != m_arguments.end()) {
            const auto result = *it;
            m_arguments.erase(it - 1, it + 1);
            return Right(result);
        }
        return Left(FlagParseError::ValueAfterFlagMissing);
    }
    return Left(FlagParseError::MandatoryFlagNotFound);
}

std::pair<Either<FlagParseError, std::string>, bool> FlagParser::popArgAfter(
    const std::string &shortName, const std::string &longName)
{
    const auto &shortVal = popArgAfter("-" + shortName);
    if (!shortVal && shortVal.left().value() == FlagParseError::MandatoryFlagNotFound) {
        return {popArgAfter("--" + longName), true};
    }
    return {shortVal, false};
}

std::vector<std::string> FlagParser::coverArgs(int argc, const char **argv)
{
    std::vector<std::string> result;
    for (int i = 0; i < argc; i++) {
        result.push_back(argv[i]);
    }
    return result;
}

bool FlagParser::finish(const HandleErrorFunc &handleErrorFunc,
                        const HandleHelpFunc &handleHelpFunc,
                        const HandleVersionFunc &handleVersionFunc)
{
    const auto help = handleHelpFunc ? flag<bool>(
                          Flag{.shortName = "h", .longName = "help", .description = "Display help"})
                                     : false;

    const auto v = handleVersionFunc ? flag<bool>(Flag{.shortName = "v",
                                                       .longName = "version",
                                                       .description = "Display version"})
                                     : false;
    if (hasErrors()) {
        handleErrorFunc(*this);
        return false;
    }
    if (help) {
        handleHelpFunc(*this);
        return true;
    }
    if (v) {
        handleVersionFunc(*this);
    }
    return true;
}

bool FlagParser::hasErrors() const
{
    if (m_arguments.size() > 1)
        return true;
    for (const auto &f : m_parsedFlags) {
        if (f.err) {
            return true;
        }
    }
    return false;
}

void FlagParser::displayErr(std::ostream &stream) const
{
    for (const auto &f : m_parsedFlags) {
        if (f.err) {
            const auto &app = appname();
            stream << (app ? cc::Default.wrap(*app + ": ", true) : "")
                   << cc::Red.wrap("error:", true) << " ";

            switch (*f.err) {
            case FlagParseError::MandatoryFlagNotFound:
                stream << "mandatory flag " << cc::Default.wrap("`" + f.longName + "`", true)
                       << " missing";
                break;
            case FlagParseError::ValueAfterFlagMissing:
                stream << "missing value after "
                       << cc::Default.wrap("`"
                                               + (f.isLongProvided ? "--" + f.longName
                                                                   : "-" + f.shortName)
                                               + "`",
                                           true);
                break;
            case FlagParseError::NumberParsingFailed:
                stream << "value after "
                       << cc::Default.wrap("`"
                                               + (f.isLongProvided ? "--" + f.longName
                                                                   : "-" + f.shortName)
                                               + "`",
                                           true)
                       << " must be a number";
                break;
            case FlagParseError::EnumValueNotFound:
                stream << cc::Default.wrap("`" + f.raw + "`", true)
                              + " is invalid enum value after "
                       << cc::Default.wrap("`"
                                               + (f.isLongProvided ? "--" + f.longName
                                                                   : "-" + f.shortName)
                                               + "`",
                                           true);
                break;
            default:
                stream << "unknown error";
            }

            stream << std::endl;
        }
    }

    for (size_t i = 1; i < m_arguments.size(); ++i) {
        stream << "unrecognized command-line option "
               << cc::Default.wrap("`" + m_arguments[i] + "`", true) << std::endl;
    }
}

void FlagParser::displayHelp(std::ostream &stream) const
{
    constexpr const char *tab = "  ";

    if (const auto &app = appname()) {
        stream << "Usage: " + *app + " [options]" << std::endl;
    }

    size_t maxShortSize = 0;
    size_t maxLongSize = 0;
    for (const auto &d : m_parsedFlags) {
        maxShortSize = std::max(maxShortSize, d.shortName.size() + 1 /* "-" */);
        maxLongSize = std::max(maxLongSize, d.longName.size() + 2 /* "--" */);
    }

    stream << "Options:" << std::endl;
    for (const auto &d : m_parsedFlags) {
        stream << tab << "-" << d.shortName
               << std::string(maxShortSize - d.shortName.size() - 1 /* "-" */, ' ');
        stream << (d.isBool ? "   " : " <>");
        stream << tab << "--" << d.longName
               << std::string(maxLongSize - d.longName.size() - 2 /* "--" */, ' ');
        stream << tab << d.description << std::endl;
    }
}

void FlagParser::displayVersion(std::ostream &stream, const std::string &version) const
{
    if (const auto &app = appname()) {
        stream << *app << " " << version << std::endl;
    } else {
        stream << version << std::endl;
    }
}

} // namespace e172
