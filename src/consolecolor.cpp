// Copyright 2023 Borys Boiko

#include "consolecolor.h"

std::string e172::ConsoleColor::foreground(bool bold) const
{
    if (m_foreground == 0) {
        return std::string("\x1B[") + (bold ? "1" : "0") + "m";
    } else {
        return "\x1B[" + std::to_string(m_foreground) + (bold ? ";1" : "") + "m";
    }
}

std::string e172::ConsoleColor::background() const {
    return "\x1B[" + std::to_string(m_background) + "m";
}

std::string e172::ConsoleColor::wrap(const std::string &string, bool bold) const
{
    return foreground(bold) + string + cc::Default.foreground();
}

std::string e172::ConsoleColor::wrapBackground(const std::string &string) const {
    return background() + string + cc::Default.foreground();
}

