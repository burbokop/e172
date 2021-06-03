#ifndef KSMARTENUM_H
#define KSMARTENUM_H

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace e172 {

struct __enum_tools {
    static std::string trim(const std::string& str);
    static std::vector<std::string> split(const std::string& s, char delimiter);
    static std::map<uint8_t, std::string> __va_args_to_map(const std::string &str);
};

}

#define e172_enum_class(TYPE, ...) struct TYPE { \
    enum Enum { __VA_ARGS__ }; static inline const auto names = e172::__enum_tools::__va_args_to_map(#__VA_ARGS__); \
    static inline std::string toString(Enum v) { return names.at(v); } \
};

#define e172_enum(TYPE, ...) \
    public: \
        enum TYPE { __VA_ARGS__ }; \
    private: \
        static inline const auto TYPE ## Strings2 = e172::__enum_tools::__va_args_to_map(#__VA_ARGS__); \
    public: \
        friend inline std::ostream& operator<<(std::ostream& stream, TYPE value) { \
            return stream << TYPE ## Strings2.at(value); \
        } \
    private:

#define e172_enum_member(TYPE, NAME, ...) \
    public: \
        enum TYPE { __VA_ARGS__ }; \
    private: \
        TYPE m_ ## NAME; \
        static inline const auto NAME ## Strings = e172::__enum_tools::__va_args_to_map(#__VA_ARGS__); \
    public: \
        TYPE NAME() const { return m_ ## NAME; } \
        std::string NAME ## String() const { return NAME ## Strings.at(m_ ## NAME); } \
    private:

#endif // KSMARTENUM_H
