#include "consolecolor.h"

std::string e172::ConsoleColor::foreground() const {
    return "\x1B[" + std::to_string(m_foreground) + "m";
}

std::string e172::ConsoleColor::background() const {
    return "\x1B[" + std::to_string(m_background) + "m";
}

std::string e172::ConsoleColor::wrap(const std::string &string) const {
    return foreground() + string + Reset;
}

std::string e172::ConsoleColor::wrapBackground(const std::string &string) const {
    return background() + string + Reset;
}

