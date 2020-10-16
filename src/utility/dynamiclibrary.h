#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H

#include <string>

namespace e172 {

class DynamicLibrary {
    void *m_handle = nullptr;
    DynamicLibrary(void *handle);
public:
    ~DynamicLibrary();
    bool isValid() const;

    void *symbol(const std::string& name) const;

    template<typename T>
    T symbol(const std::string& name) const { return reinterpret_cast<T>(symbol(name)); }

    static DynamicLibrary load(const std::string &path);
};

}

#endif // DYNAMICLIBRARY_H
