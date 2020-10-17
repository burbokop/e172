#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H

#include <string>
#include <list>

namespace e172 {

class DynamicLibrary {
    void *m_handle = nullptr;
    std::string m_path;
    DynamicLibrary(void *handle, const std::string &path);
public:
    ~DynamicLibrary();
    bool isValid() const;

    void *symbol(const std::string& name) const;

    template<typename T>
    T symbol(const std::string& name) const { return reinterpret_cast<T>(symbol(name)); }

    std::list<std::string> polymorphicSymbols(const std::string &name) const;


    static DynamicLibrary load(const std::string &path);
    std::string path() const;
};

}

#endif // DYNAMICLIBRARY_H
