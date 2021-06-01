#include "closableoutputstream.h"

e172::ClosableOutputStream::ClosableOutputStream(std::streambuf *buf, bool autoDestroyBuf) : std::ostream(buf) {
    m_buf = buf;
    m_autoDestroyBuf = autoDestroyBuf;
}

e172::ClosableOutputStream::~ClosableOutputStream() {
    if(m_autoDestroyBuf && m_buf) {
        delete m_buf;
    }
}

e172::CallbackOutputStream::CallbackOutputStream(const std::function<void (const std::string &)> &write, const std::function<void ()> &close)
    : ClosableOutputStream(new Buffer(write), true), m_close(close) {}

e172::CallbackOutputStream::Buffer::Buffer(const std::function<void (const std::string &)> &write) {
    m_write = write;
}

void e172::CallbackOutputStream::close() {
    if(m_close) {
        m_close();
    }
}

std::streamsize e172::CallbackOutputStream::Buffer::xsputn(const std::streambuf::char_type *s, std::streamsize n) {
    if(m_write) {
        m_write(std::string(s, n));
    }
    return n;
}

std::streambuf::int_type e172::CallbackOutputStream::Buffer::overflow(std::streambuf::int_type c) {
    if (c == traits_type::eof()) {
        return traits_type::eof();
    } else {
        char_type ch = traits_type::to_char_type(c);
        return xsputn(&ch, 1) == 1 ? c : traits_type::eof();
    }
}
