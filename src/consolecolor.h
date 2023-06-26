// Copyright 2023 Borys Boiko

#pragma once

#include <string>
#include <sstream>

namespace e172 {

class ConsoleColor {
public:
    constexpr ConsoleColor(int foreground = 0, int background = 0)
        : m_foreground(foreground)
        , m_background(background)
    {}

    std::string foreground(bool bold = false) const;
    std::string background() const;
    inline operator std::string() const { return foreground(); }

    friend std::ostream &operator<<(std::ostream &stream, const ConsoleColor &color)
    {
        return stream << color.foreground();
    }

    std::string wrap(const std::string &string, bool bold = false) const;
    std::string wrapBackground(const std::string &string) const;
    std::string operator()(const std::string &string) const { return wrap(string); }

    template<typename T>
    std::string wrap(const T &value) const {
        std::stringstream ss;
        ss << value;
        return wrap(ss.str());
    }

    template<typename T>
    std::string wrapBackground(const T &value) const
    {
        std::stringstream ss;
        ss << value;
        return wrapBackground(ss.str());
    }

    template<typename T>
    std::string operator()(const T &value) const
    {
        return wrap<T>(value);
    }

private:
    std::uint8_t m_foreground = 0;
    std::uint8_t m_background = 0;
};

/**
 * Console colors 
 */
namespace cc {

constexpr ConsoleColor Default = {0, 0};

constexpr ConsoleColor Black = {30, 40};
constexpr ConsoleColor Red = {31, 41};
constexpr ConsoleColor Green = {32, 42};
constexpr ConsoleColor Yellow = {33, 43};
constexpr ConsoleColor Blue = {34, 44};
constexpr ConsoleColor Magenta = {35, 45};
constexpr ConsoleColor Cyan = {36, 46};
constexpr ConsoleColor White = {37, 47};
constexpr ConsoleColor BrightBlack = {90, 100};
constexpr ConsoleColor BrightRed = {91, 101};
constexpr ConsoleColor BrightGreen = {92, 102};
constexpr ConsoleColor BrightYellow = {93, 103};
constexpr ConsoleColor BrightBlue = {94, 104};
constexpr ConsoleColor BrightMagenta = {95, 105};
constexpr ConsoleColor BrightCyan = {96, 106};
constexpr ConsoleColor BrightWhite = {97, 107};

} // namespace cc

} // namespace e172
