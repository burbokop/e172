#ifndef DYNAMICLIBRARY_H
#define DYNAMICLIBRARY_H

#include "destroysignal.h"

#include <string>
#include <list>
#include <memory>

namespace e172 {


template <typename T>
struct FunctionGhost {};
template <typename T, typename ...Args>
struct FunctionGhost<T(Args...)> {};


template <typename T>
class DynamicLibraryFunction {};

template <typename T, typename ...Args>
class DynamicLibraryFunction<T(Args...)> {
    friend class DynamicLibrary;
    std::function<T(Args...)> m_function;
    std::shared_ptr<DestroySignal> m_signal;
    DynamicLibraryFunction(const std::function<T(Args...)> &function, const std::shared_ptr<DestroySignal> &signal) {
        m_function = function;
        m_signal = signal;
    };
public:
    DynamicLibraryFunction() {};
    operator bool() const { return static_cast<bool>(m_function); }
    T operator()(Args... args) const { return m_function(args...); }
};


class DynamicLibrary {
    void *m_handle = nullptr;
    std::string m_path;

    std::shared_ptr<DestroySignal> m_destroySignal;
    DynamicLibrary(void *handle, const std::string &path, const std::shared_ptr<DestroySignal> &destroySignal);
    void *__symbol(const std::string& name) const;



    template <typename T, typename ...Args>
    DynamicLibraryFunction<T(Args...)> symbol(const FunctionGhost<T(Args...)> &, const std::string& name) const {
        return DynamicLibraryFunction<T(Args...)>(reinterpret_cast<T(*)(Args...)>(__symbol(name)), m_destroySignal);
    }
public:

    bool isValid() const;


    template <typename T>
    DynamicLibraryFunction<T> symbol(const std::string& name) const {
        return symbol(FunctionGhost<T>(), name);
    }

    std::list<std::string> polymorphicSymbols(const std::string &name) const;

    static DynamicLibrary load(const std::string &path);
    std::string path() const;
};

}

#endif // DYNAMICLIBRARY_H
