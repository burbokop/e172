#ifndef SIGNALSTREAMBUFFER_H
#define SIGNALSTREAMBUFFER_H

#include <streambuf>
#include <functional>
#include <list>

namespace e172 {

class SignalStreamBuffer : public std::streambuf {
public:
    typedef std::function<void(const std::string &)> HandlerFunc;
private:
    std::list<HandlerFunc> m_handleFuncs;
public:
    SignalStreamBuffer();
    void connect(const HandlerFunc& func);
    void connect(std::ostream& stream);
    void disconnectAll();

    // basic_streambuf interface
protected:
    virtual std::streamsize xsputn(const char_type *s, std::streamsize n) override;
    virtual int_type overflow(int_type c) override;
};

}

#endif // SIGNALSTREAMBUFFER_H
