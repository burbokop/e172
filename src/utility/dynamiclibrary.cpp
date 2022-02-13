#include "dynamiclibrary.h"
#include <dlfcn.h>
#include <src/additional.h>

std::string e172::DynamicLibrary::path() const {
    return m_path;
}

e172::DynamicLibrary::DynamicLibrary(void *handle, const std::string &path, const std::shared_ptr<Defer> &destroySignal) {
    m_handle = handle;
    m_path = path;
    m_destroySignal = destroySignal;
}

bool e172::DynamicLibrary::isValid() const {
    return m_handle;
}

void *e172::DynamicLibrary::__symbol(const std::string &name) const {
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

std::list<std::string> e172::DynamicLibrary::polymorphicSymbols(const std::string &name) const {
    const auto stdout = e172::Additional::executeCommand(("objdump --syms " + m_path + " | grep \" _Z" + std::to_string(name.size()) + name + '"').c_str());
    std::list<std::string> result;
    for(const auto& s : stdout) {
        const auto l = e172::Additional::split(s, ' ');
        if(l.size() > 0) {
            result.push_back(e172::Additional::trim(l[l.size() - 1]));
        }
    }
    return result;
}

e172::DynamicLibrary e172::DynamicLibrary::load(const std::string &path) {
#ifdef __unix__
    const auto handle = dlopen(path.c_str(), RTLD_LAZY);
    return DynamicLibrary(handle, path, std::make_shared<Defer>([handle]{ dlclose(handle); }));
#else
    return DynamicLibrary(nullptr, path, nullptr);
#endif
}
