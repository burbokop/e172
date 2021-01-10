#include "flagparser.h"
#include "../additional.h"
#include "../variant.h"

e172::FlagParser::FlagParser() {}

e172::FlagParser::FlagParser(int argc, char *argv[]) : FlagParser(Additional::coverArgs(argc, argv)) {}

e172::FlagParser::FlagParser(const std::vector<std::string> &args) {
    m_arguments = args;
    registerBoolFlag("-h", "--help", "Display help");
}

void e172::FlagParser::registerValueFlag(const std::string &shortName, const std::string &fullName, const std::string &description) {
    m_flagsDescription.push_back({ shortName, fullName, description, true });

    const auto short_flag = nextArg(shortName);
    if(short_flag.size() > 0) {
        m_flags[shortName] = short_flag;
        return;
    } else if(fullName.size() > 0) {
        const auto full_flag = nextArg(fullName);
        if(full_flag.size() > 0) {
            m_flags[shortName] = full_flag;
            return;
        }
    }
}

void e172::FlagParser::registerBoolFlag(const std::string &shortName, const std::string &fullName, const std::string &description) {
    m_flagsDescription.push_back({ shortName, fullName, description, false });

    if(containsArg(shortName)) {
        m_flags[shortName] = true;
        return;
    } else if(fullName.size() > 0) {
        if(containsArg(fullName)) {
            m_flags[shortName] = true;
            return;
        }
    }
}

void e172::FlagParser::displayHelp(std::ostream &stream) {
    constexpr const char* tab = "  ";

    if(m_arguments.size() > 0) {
        stream << "Usage: " + m_arguments[0] + " [options]\n";
    }

    if(m_flagsDescription.size() <= 1) {
        stream << "No options specified\n";
        return;
    }

    size_t maxSnSize = 0;
    size_t maxFnSize = 0;
    for(const auto& d : m_flagsDescription) {
        maxSnSize = std::max(maxSnSize, d.shortName.size());
        maxFnSize = std::max(maxFnSize, d.fullName.size());
    }

    stream << "Options:\n";
    for(const auto& d : m_flagsDescription) {
        stream << tab << d.shortName << std::string(maxSnSize - d.shortName.size(), ' ');
        stream << (d.isValueFlag ? " <>" : "   ");
        stream << tab << d.fullName << std::string(maxFnSize - d.fullName.size(), ' ');
        stream << tab << d.description << '\n';
    }
}

e172::VariantMap e172::FlagParser::flags() const {
    return m_flags;
}

std::vector<std::string> e172::FlagParser::arguments() const {
    return m_arguments;
}

bool e172::FlagParser::containsFlag(const std::string &shortName) const {
    return m_flags.contains(shortName);
}

e172::Variant e172::FlagParser::flag(const std::string &shortName) const {
    const auto it = m_flags.find(shortName);
    if(it == m_flags.end()) {
        return Variant();
    } else {
        return it->second;
    }
}

bool e172::FlagParser::containsArg(const std::string &arg) const {
    auto it = std::find(m_arguments.begin(), m_arguments.end(), arg);
    return it != m_arguments.end();
}

std::string e172::FlagParser::nextArg(const std::string &arg) const {
    auto it = std::find(m_arguments.begin(), m_arguments.end(), arg);
    if(it != m_arguments.end()) {
        it++;
        if(it != m_arguments.end()) {
            return *it;
        }
    }
    return {};
}

