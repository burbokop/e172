// Copyright 2023 Borys Boiko

#pragma once

#include "../typedefs.h"
#include <list>
#include <string>
#include <vector>

namespace e172 {

class FlagParser {
public:
    FlagParser() = default;
    FlagParser(int argc, const char *argv[]);
    FlagParser(const std::vector<std::string> &args);
    void registerValueFlag(const std::string &shortName,
                           const std::string &fullName = std::string(),
                           const std::string &description = std::string());

    void registerBoolFlag(const std::string &shortName,
                          const std::string &fullName = std::string(),
                          const std::string &description = std::string());

    void displayHelp(std::ostream &stream);
    const VariantMap &flags() const { return m_flags; }
    const std::vector<std::string> &arguments() const { return m_arguments; }

    bool containsFlag(const std::string &shortName) const;
    Variant flag(const std::string &shortName) const;

private:
    struct Description
    {
        std::string shortName;
        std::string fullName;
        std::string description;
        bool isValueFlag;
    };

    std::string nextArg(const std::string &arg) const;
    bool containsArg(const std::string &arg) const;

private:
    VariantMap m_flags;
    std::list<Description> m_flagsDescription;
    std::vector<std::string> m_arguments;
};

} // namespace e172
