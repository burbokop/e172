#include "signalstreambuffer.h"

e172::SignalStreamBuffer::SignalStreamBuffer() {}

void e172::SignalStreamBuffer::connect(const SignalStreamBuffer::HandlerFunc &func) {
    m_handleFuncs.push_back(func);
}

std::streamsize e172::SignalStreamBuffer::xsputn(const std::streambuf::char_type *s, std::streamsize n) {
    const auto& string = std::string(s, n);
    for(const auto& f : m_handleFuncs) {
        f(string);
    }
    return n;
}

std::streambuf::int_type e172::SignalStreamBuffer::overflow(std::streambuf::int_type c) {
    if (c == traits_type::eof()) {
        return traits_type::eof();
    } else {
        char_type ch = traits_type::to_char_type(c);
        return xsputn(&ch, 1) == 1 ? c : traits_type::eof();
    }
}

void e172::SignalStreamBuffer::connect(std::ostream &stream) {
    connect([&stream](const std::string& str){ stream << str; });
}

void e172::SignalStreamBuffer::disconnectAll() {
    m_handleFuncs.clear();
}
