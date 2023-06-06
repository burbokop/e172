#include "additional.h"
#include "debug.h"
#include "type.h"

#include <iostream>

#include <signal.h>    // for signal
#include <execinfo.h>  // for backtrace
#include <dlfcn.h>     // for dladdr

namespace e172 {



std::function<void(const std::string &, Debug::MessageType)> Debug::m_proceedMessage = [](const std::string &data, Debug::MessageType type){
    if(type == Debug::PrintMessage) {
        std::cout << data << std::endl;
    } else if(type == Debug::WarningMessage) {
        std::cout << "\033[33m" << data << "\033[0m" << std::endl;
    } else if(type == Debug::FatalMessage) {
        std::cerr << data << std::endl;
        exit(1);
    }
};

int Debug::functionName(void *addr, std::string *fname, std::string *sname) {
#ifdef __unix__
    Dl_info info;
    int res = dladdr(addr, &info);
    *fname = info.dli_fname;
    *sname = info.dli_sname;
    return res;
#else
    (void)addr;
    return -100;
#endif
}

std::list<StackTraceInfo> Debug::stackTrace() {
    std::vector<std::string> st;
#ifdef __unix__
    void *addrlist[64];
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));
    char **symbollist = backtrace_symbols(addrlist, addrlen);

    st.resize(addrlen);
    for (int j = 0; j < addrlen; j++) {
        st[j] = symbollist[j];
    }

    free(symbollist);
#endif
    std::list<StackTraceInfo> result;
    for(auto sti : st) {
        StackTraceInfo info;
        const auto p0 = Additional::split(sti, '(');
        if(p0.size() > 1) {
            info.m_libPath = p0[0];
            info.m_libName = Additional::pathTopLevelItem(info.m_libPath);
            const auto p1 = Additional::split(p0[1], ')');
            if(p1.size() > 1) {
                const auto p2 = Additional::split(p1[0], '+');
                if(p2.size() > 0) {
                    info.m_functionName = p2[0];
                    if(info.m_functionName.size() > 0) {
                        try {
                            info.m_functionName = TypeTools::demangle(info.m_functionName);
                        } catch (std::runtime_error) {}
                    }
                    if(p2.size() > 1) {
                        size_t pos = 2;
                        info.m_offset = std::stoul(p2[1], &pos, 16);
                    }
                }
                auto address = Additional::fencedArea(p1[1], Additional::Fence::Brackets);
                if(address.size() > 2) {
                    address = address.substr(1, address.size() - 2);
                }
                size_t pos = 2;
                info.m_address = std::stoul(address, &pos, 16);
            }
        }
        result.push_back(info);
    }
    return result;
}

void Debug::installSigsegvHandler(void(*function)(int)) {
#ifdef __unix__
    signal(SIGSEGV, function);
#endif
}

std::string Debug::codeLocation(const char *file, int line) {
    return std::string(file) + ":" + std::to_string(line) + ":";
}

std::string Debug::makeVersion(int a, int b, int c)
{
    std::ostringstream ss;
    ss << a << '.' << b << '.' << c;
    return ss.str();
}

Debug::CompilerInfo Debug::compilerInfo() {
    return CompilerInfo(cxx, cxx_version);
}

void Debug::installHandler(const std::function<void (const std::string &, Debug::MessageType)> &handler) {
    m_proceedMessage = handler;
}

std::string StackTraceInfo::libName() const {
    return m_libName;
}

std::string StackTraceInfo::libPath() const {
    return m_libPath;
}

uintptr_t StackTraceInfo::address() const {
    return m_address;
}

uintptr_t StackTraceInfo::offset() const {
    return m_offset;
}

std::ostream &operator<<(std::ostream &stream, const StackTraceInfo &info) {
    stream  << '{'
            << (info.m_functionName.size() > 0 ? info.m_functionName : "<anonymus>")
            << ", "
            << info.m_libName
            << ", "
            << "0x" << std::hex << info.m_address
            << '}';
    return stream;
}

std::string StackTraceInfo::functionName() const {
    return m_functionName;
}

std::string Debug::CompilerInfo::name() const {
    return m_name;
}

std::string Debug::CompilerInfo::version() const {
    return m_version;
}

std::ostream &operator<<(std::ostream &stream, const Debug::CompilerInfo &info) {
    return stream << info.m_name + "-" + info.m_version;
}

Debug::CompilerInfo::CompilerInfo(const std::string &name, const std::string &version)
    : m_name(name), m_version(version) {}



}
