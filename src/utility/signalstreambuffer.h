// Copyright 2023 Borys Boiko

#pragma once

#include <functional>
#include <list>
#include <streambuf>
#include <string>

namespace e172 {

class SignalStreamBuffer : public std::streambuf {
public:
    using HandlerFunc = std::function<void(const std::string &)>;

public:
    SignalStreamBuffer();
    void connect(const HandlerFunc& func);
    void connect(std::ostream& stream);
    void disconnectAll();

    // basic_streambuf interface
protected:
    std::streamsize xsputn(const char_type *s, std::streamsize n) override;
    int_type overflow(int_type c) override;

private:
    std::list<HandlerFunc> m_handleFuncs;
};

} // namespace e172
