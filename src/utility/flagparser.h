#ifndef FLAGPARSER_H
#define FLAGPARSER_H

#include "../typedefs.h"

namespace e172 {

class FlagParser {
    VariantMap m_flags;
    struct Description {
        std::string shortName;
        std::string fullName;
        std::string description;
        bool isValueFlag;
    };

    std::list<Description> m_flagsDescription;
    std::vector<std::string> m_arguments;

    std::string nextArg(const std::string& arg) const;
    bool containsArg(const std::string& arg) const;
public:
    FlagParser();
    FlagParser(int argc, char *argv[]);
    FlagParser(const std::vector<std::string> &args);
    void registerValueFlag(const std::string& shortName, const std::string& fullName = std::string(), const std::string& description = std::string());
    void registerBoolFlag(const std::string& shortName, const std::string& fullName = std::string(), const std::string& description = std::string());
    void displayHelp(std::ostream& stream);
    VariantMap flags() const;
    std::vector<std::string> arguments() const;

    bool containsFlag(const std::string &shortName) const;
    Variant flag(const std::string &shortName) const;
};

}

#endif // FLAGPARSER_H
