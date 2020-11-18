#ifndef DEBUG_H
#define DEBUG_H


#include <sstream>
#include <functional>
#include <list>

namespace e172 {


struct StackTraceInfo {
    friend class Debug;
    std::string m_functionName;
    std::string m_libName;
    std::string m_libPath;
    uintptr_t m_address;
    uintptr_t m_offset;
public:
    std::string functionName() const;
    std::string libName() const;
    std::string libPath() const;
    uintptr_t address() const;
    uintptr_t offset() const;
    friend std::ostream &operator<<(std::ostream &stream, const StackTraceInfo &info);
};



class Debug {
public:
    enum MessageType {
        PrintMessage,
        WarningMessage,
        FatalMessage
    };
private:

    static std::function<void(const std::string &, MessageType)> m_proceedMessage;
    static std::string m_separator;
public:
    static void installHandler(const std::function<void(const std::string &, MessageType)> &handler);
    static void setSeparator(const std::string &separator);


    template <typename Separator, typename Arg, typename... Args>
    static void passArgsToStream(std::ostream& out, const Separator &separator, const Arg& arg, const Args& ...args) {
        out << arg;
        ((out << separator << args), ...);
    }


    template<typename Arg, typename ...Args>
    static void emitMessage(MessageType type, const Arg& arg, const Args& ...args) {
        if(m_proceedMessage) {
            std::stringstream ss;
            passArgsToStream(ss, m_separator, arg, args...);
            m_proceedMessage(ss.str(), type);
        }
    }


    template<typename Arg, typename ...Args>
    static void print(const Arg& arg, const Args& ...args) {
        emitMessage(PrintMessage, arg, args...);
    }

    template<typename Arg, typename ...Args>
    static void warning(const Arg& arg, const Args& ...args) {
        emitMessage(WarningMessage, arg, args...);
    }

    template<typename Arg, typename ...Args>
    static void fatal(const Arg& arg, const Args& ...args) {
        emitMessage(FatalMessage, arg, args...);
    }

    static int functionName(void *addr, std::string *fname, std::string *sname);

    static std::list<StackTraceInfo> stackTrace();
    static void installSigsegvHandler(void(*function)(int));


    static inline std::string make_version(int a, int b, int c) {
        std::ostringstream ss;
        ss << a << '.' << b << '.' << c;
        return ss.str();
    }

    static inline const std::string cxx =
#ifdef __clang__
            "clang++";
#else
            "g++";
#endif

    static inline const std::string cxx_version =
#ifdef __clang__
            make_version(__clang_major__, __clang_minor__, __clang_patchlevel__);
#else
            make_version(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif

};

}


#endif // DEBUG_H
