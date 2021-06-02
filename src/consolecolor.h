#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H

#include <string>
#include <sstream>

namespace e172 {

class ConsoleColor {
    int m_foreground = 0;
    int m_background = 0;
public:
    ConsoleColor(int foreground = 0, int background = 0);
    std::string foreground() const;
    std::string background() const;
    inline operator std::string() const { return foreground(); }

    friend std::ostream &operator <<(std::ostream& stream, const ConsoleColor& color);
    std::string wrap(const std::string &string) const;
    std::string wrapBackground(const std::string &string) const;
    std::string operator()(const std::string &string) const { return wrap(string); }

    template<typename T>
    std::string wrap(const T &value) const {
        std::stringstream ss;
        ss << value;
        return wrap(ss.str());
    }
    template<typename T>
    std::string wrapBackground(const T &value) const {
        std::stringstream ss;
        ss << value;
        return wrapBackground(ss.str());
    }
    template<typename T>
    std::string operator()(const T &value) const { return wrap<T>(value); }
};

static inline const std::string Reset = "\033[0m";

static inline const ConsoleColor Black          = { 30, 40 };
static inline const ConsoleColor Red            = { 31, 41 };
static inline const ConsoleColor Green          = { 32, 42 };
static inline const ConsoleColor Yellow         = { 33, 43 };
static inline const ConsoleColor Blue           = { 34, 44 };
static inline const ConsoleColor Magenta        = { 35, 45 };
static inline const ConsoleColor Cyan           = { 36, 46 };
static inline const ConsoleColor White          = { 37, 47 };
static inline const ConsoleColor BrightBlack   = { 90, 100 };
static inline const ConsoleColor BrightRed     = { 91, 101 };
static inline const ConsoleColor BrightGreen   = { 92, 102 };
static inline const ConsoleColor BrightYellow  = { 93, 103 };
static inline const ConsoleColor BrightBlue    = { 94, 104 };
static inline const ConsoleColor BrightMagenta = { 95, 105 };
static inline const ConsoleColor BrightCyan    = { 96, 106 };
static inline const ConsoleColor BrightWhite   = { 97, 107 };

}
#endif // CONSOLECOLOR_H
