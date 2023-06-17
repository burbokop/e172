// Copyright 2023 Borys Boiko

#pragma once

#include <functional>
#include <ostream>
#include <streambuf>
#include <string>

namespace e172 {

class ClosableOutputStream : public std::ostream {
public:
    ClosableOutputStream(std::streambuf *buf, bool autoDestroyBuf = false);
    ~ClosableOutputStream();

    virtual void close() = 0;

private:
    std::streambuf *m_buf = nullptr;
    bool m_autoDestroyBuf = false;
};

class CallbackOutputStream : public ClosableOutputStream {
public:
    class SingleElementPool
    {
    public:
        SingleElementPool(const std::function<void(const std::string &)> &write = nullptr)
            : m_write(write)
        {}

        CallbackOutputStream *use();
        bool available() const { return m_available; }
        ~SingleElementPool();

    private:
        CallbackOutputStream *m_stream = nullptr;
        bool m_available = true;
        std::function<void(const std::string &)> m_write;
    };

    CallbackOutputStream(const std::function<void(const std::string &)> &write = nullptr,
                         const std::function<void()> &close = nullptr);

    void close() override;

private:
    class Buffer : public std::streambuf
    {
    public:
        Buffer(const std::function<void(const std::string &)> &write)
            : m_write(write)
        {}

        // basic_streambuf interface
    protected:
        std::streamsize xsputn(const char_type *s, std::streamsize n) override;
        int_type overflow(int_type c) override;

    private:
        std::function<void(const std::string &)> m_write;
    };

private:
    std::function<void()> m_close;
};

} // namespace e172
