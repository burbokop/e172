#ifndef CLOSABLEOUTPUTSTREAM_H
#define CLOSABLEOUTPUTSTREAM_H

#include <ostream>
#include <streambuf>
#include <functional>

namespace e172 {

class ClosableOutputStream : public std::ostream {
    std::streambuf *m_buf = nullptr;
    bool m_autoDestroyBuf = false;
public:
    ClosableOutputStream(std::streambuf *buf, bool autoDestroyBuf = false);
    virtual ~ClosableOutputStream() override;

    virtual void close() = 0;
};

class CallbackOutputStream : public ClosableOutputStream {
    class Buffer : public std::streambuf {
        std::function<void(const std::string&)> m_write;
    public:
        Buffer(const std::function<void(const std::string&)>& write);
        // basic_streambuf interface
    protected:
        virtual std::streamsize xsputn(const char_type *s, std::streamsize n) override;
        virtual int_type overflow(int_type c) override;
    };
    std::function<void()> m_close;
public:
    CallbackOutputStream(const std::function<void(const std::string&)>& write = nullptr, const std::function<void()>& close = nullptr);

    virtual void close() override;

    class SingleElementPool {
        CallbackOutputStream *m_stream = nullptr;
        bool m_available = true;
        std::function<void(const std::string&)> m_write;
    public:
        SingleElementPool(const std::function<void(const std::string&)>& write = nullptr);
        CallbackOutputStream *use();
        bool available() const;
        ~SingleElementPool();
    };
};

}

#endif // CLOSABLEOUTPUTSTREAM_H
