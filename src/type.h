#pragma once

#if __has_include(<boost/core/demangle.hpp>)
#include <boost/core/demangle.hpp>

namespace e172 {

namespace TypeTools {
inline std::string demangle(const std::string &typeName) { return boost::core::demangle(typeName.c_str()); }
}

template <typename T>
struct Type {
    static inline std::string name() { return boost::core::demangle(typeid(T).name()); }
    static inline size_t hash() { return typeid(T).hash_code(); }
};
}

#else

#include <typeinfo>
#include <string>

namespace e172 {

namespace TypeTools {
std::string demangle(const std::string &typeName);
}

template <typename T>
class Type {
    static inline const std::string m_name = TypeTools::demangle(typeid (T).name());
    static inline const size_t m_hash = typeid (T).hash_code();
public:
    static inline std::string name() { return m_name.size() > 0 ? m_name : TypeTools::demangle(typeid (T).name()); }
    static inline size_t hash() { return m_hash ? m_hash : typeid (T).hash_code(); }
};


}
#endif
