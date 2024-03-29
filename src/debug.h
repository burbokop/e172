// Copyright 2023 Borys Boiko

#pragma once

#include <functional>
#include <list>
#include <sstream>
#include <string>

namespace e172 {


struct StackTraceInfo {
    friend class Debug;
public:
    std::string functionName() const;
    std::string libName() const { return m_libName; }
    std::string libPath() const { return m_libPath; }
    std::uintptr_t address() const { return m_address; }
    std::uintptr_t offset() const { return m_offset; }
    friend std::ostream &operator<<(std::ostream &stream, const StackTraceInfo &info);

private:
    std::string m_functionName;
    std::string m_libName;
    std::string m_libPath;
    std::uintptr_t m_address;
    std::uintptr_t m_offset;
};

class Debug
{
public:
    enum MessageType { PrintMessage, WarningMessage, FatalMessage };

    using Handler = std::function<void(const std::string &, MessageType)>;

    class CompilerInfo {
    public:
        CompilerInfo(const std::string &name, const std::string &version)
            : m_name(name)
            , m_version(version)
        {}

        std::string name() const { return m_name; }
        std::string version() const { return m_version; }
        friend std::ostream &operator<<(std::ostream &stream, const CompilerInfo &info);

    private:
        std::string m_name;
        std::string m_version;
    };

    static Debug withSepar(const std::string &str) { return Debug(str); }

    static CompilerInfo compilerInfo();

    static void installHandler(const Handler &handler);

    template <typename Separator, typename Arg, typename... Args>
    static void passArgsToStream(std::ostream& out, const Separator &separator, const Arg& arg, const Args& ...args) {
        out << arg;
        ((out << separator << args), ...);
    }

    template<typename Arg, typename... Args>
    void emitMessage(MessageType type, const Arg &arg, const Args &...args)
    {
        std::stringstream ss;
        passArgsToStream(ss, m_separ, arg, args...);
        handle(ss.str(), type);
    }

    template<typename Arg, typename... Args>
    static void print(const Arg &arg, const Args &...args)
    {
        Debug().emitMessage(PrintMessage, arg, args...);
    }

    template<typename Arg, typename... Args>
    static void warning(const Arg &arg, const Args &...args)
    {
        Debug().emitMessage(WarningMessage, arg, args...);
    }

    template<typename Arg, typename... Args>
    static void fatal(const Arg &arg, const Args &...args)
    {
        Debug().emitMessage(FatalMessage, arg, args...);
    }

    static int functionName(void *addr, std::string *fname, std::string *sname);
    static std::list<StackTraceInfo> stackTrace();
    static void installSigsegvHandler(void(*function)(int));
    static std::string codeLocation(const char *file, int line);

private:
    Debug(const std::string &separ = " ")
        : m_separ(separ)
    {}

    static std::string makeVersion(int a, int b, int c);

private:
    static Handler m_handler;

    static void handle(const std::string &, MessageType);

    static inline const std::string cxx =
#if defined(__clang__)
        "clang++";
#elif defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER)
        "msvc";
#elif defined(__GNUC__)
        "g++";
#else
        "unknown";
#endif

    static inline const std::string cxx_version =
#if defined(__clang__)
        makeVersion(__clang_major__, __clang_minor__, __clang_patchlevel__);
#elif defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER)
        makeVersion((_MSC_FULL_VER / 10000000) % 100, (_MSC_FULL_VER / 100000) % 100, _MSC_FULL_VER % 100000);
#elif defined(__GNUC__)
        makeVersion(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#else
        makeVersion(0, 0, 0);
#endif

    std::string m_separ;
};

template<typename Arg, typename ...Args>
static void print(const Arg& arg, const Args& ...args) {
    Debug::print(arg, args...);
}

template<typename Arg, typename ...Args>
static void warning(const Arg& arg, const Args& ...args) {
    Debug::warning(arg, args...);
}

template<typename Arg, typename ...Args>
static void fatal(const Arg& arg, const Args& ...args) {
    Debug::fatal(arg, args...);
}

} // namespace e172
