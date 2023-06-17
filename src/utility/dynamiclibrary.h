// Copyright 2023 Borys Boiko

#pragma once

#include "defer.h"
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
public:
    DynamicLibraryFunction() {};
    operator bool() const { return static_cast<bool>(m_function); }
    T operator()(Args... args) const { return m_function(args...); }

private:
    DynamicLibraryFunction(const std::function<T(Args...)> &function,
                           const std::shared_ptr<Defer> &destroySignal)
    {
        m_function = function;
        m_destroySignal = destroySignal;
    };

private:
    std::function<T(Args...)> m_function;
    std::shared_ptr<Defer> m_destroySignal;
};

class DynamicLibrary {
public:
    bool isValid() const { return m_handle; }

    template <typename T>
    DynamicLibraryFunction<T> symbol(const std::string& name) const {
        return symbol(FunctionGhost<T>(), name);
    }

    std::list<std::string> polymorphicSymbols(const std::string &name) const;

    static DynamicLibrary load(const std::string &path);
    std::string path() const { return m_path; }

private:
    DynamicLibrary(void *handle,
                   const std::string &path,
                   const std::shared_ptr<Defer> &destroySignal)
        : m_handle(handle)
        , m_path(path)
        , m_destroySignal(destroySignal)
    {}

    void *rawSymbol(const std::string &name) const;

    template<typename T, typename... Args>
    DynamicLibraryFunction<T(Args...)> symbol(const FunctionGhost<T(Args...)> &,
                                              const std::string &name) const
    {
        return DynamicLibraryFunction<T(Args...)>(reinterpret_cast<T (*)(Args...)>(rawSymbol(name)),
                                                  m_destroySignal);
    }

private:
    void *m_handle = nullptr;
    std::string m_path;
    std::shared_ptr<Defer> m_destroySignal;
};

} // namespace e172
