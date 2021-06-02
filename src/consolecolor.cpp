#include "consolecolor.h"


e172::ConsoleColor::ConsoleColor(int foreground, int background) {
    m_foreground = foreground;
    m_background = background;
}

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

std::ostream &e172::operator <<(std::ostream &stream, const e172::ConsoleColor &color) {
    return stream << color.foreground();
}

