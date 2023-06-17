// Copyright 2023 Borys Boiko

#pragma once

#if __has_include(<boost/core/demangle.hpp>)
#include <boost/core/demangle.hpp>

namespace e172 {

namespace TypeTools {

inline std::string demangle(const std::string &typeName)
{
    return boost::core::demangle(typeName.c_str());
}

} // namespace TypeTools

template<typename T>
class Type
{
public:
    static std::string name() { return boost::core::demangle(typeid(T).name()); }
    static size_t hash() { return typeid(T).hash_code(); }
};

} // namespace e172

#else

#include <typeinfo>
#include <string>

namespace e172 {

namespace TypeTools {
std::string demangle(const std::string &typeName);
}

template <typename T>
class Type {
public:
    static std::string name()
    {
        return s_name.size() > 0 ? s_name : TypeTools::demangle(typeid(T).name());
    }

    static std::size_t hash() { return s_hash ? s_hash : typeid(T).hash_code(); }

private:
    static inline const std::string s_name = TypeTools::demangle(typeid(T).name());
    static inline const std::size_t s_hash = typeid(T).hash_code();
};

} // namespace e172

#endif
