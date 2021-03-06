#include "type.h"


#if !__has_include(<boost/core/demangle.hpp>)

#include <cxxabi.h>
#include <stdexcept>

std::string e172::TypeTools::demangle(const std::string &typeName) {
    size_t originalTypeNameSize = typeName.size();
    int status = 0;
    char *demangleResult = abi::__cxa_demangle(typeName.c_str(), nullptr, &originalTypeNameSize, &status);
    std::string result;
    if(status == 0) {
        result = demangleResult;
    } else if(status == -1) {
        throw std::runtime_error("e172::demangle: Memory allocation failiure occurred.");
    } else if(status == -2) {
        throw std::runtime_error("e172::demangle: Mangled_name is not a valid name under the C++ ABI mangling rules. (mangled: " + typeName + ")");
    } else if(status == -3) {
        throw std::runtime_error("e172::demangle: One of the arguments is invalid.");
    }
    std::free(demangleResult);
    return result;
}
#endif
