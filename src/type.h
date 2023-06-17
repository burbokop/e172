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
        return m_name.size() > 0 ? m_name : TypeTools::demangle(typeid(T).name());
    }

    static size_t hash() { return m_hash ? m_hash : typeid(T).hash_code(); }

private:
    static const std::string m_name = TypeTools::demangle(typeid(T).name());
    static const size_t m_hash = typeid(T).hash_code();
};

} // namespace e172

#endif
