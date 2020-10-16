#include "dynamiclibrary.h"
#include <dlfcn.h>

e172::DynamicLibrary::DynamicLibrary(void *handle) {
    m_handle = handle;
}

e172::DynamicLibrary::~DynamicLibrary() {
#ifdef __unix__
    dlclose(m_handle);
#endif
}

bool e172::DynamicLibrary::isValid() const {
    return m_handle;
}

void *e172::DynamicLibrary::symbol(const std::string &name) const {
#ifdef __unix__
    if(m_handle) {
        const auto fn = dlsym(m_handle, name.c_str());
        if (dlerror() == NULL) {
            return fn;
        }
    }
#endif
    (void)name;
    return nullptr;
}

e172::DynamicLibrary e172::DynamicLibrary::load(const std::string &path) {
#ifdef __unix__
    return dlopen(path.c_str(), RTLD_LAZY);
#else
    return DynamicLibrary(nullptr);
#endif
}
