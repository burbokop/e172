#pragma once

#include <stdexcept>

namespace e172 {

class UnimplementedException : public std::logic_error
{
public:
    UnimplementedException(const char *function)
        : std::logic_error("Not Implemented")
        , m_message(std::string(function) + " not implemented"){};

    virtual const char *what() const throw() { return m_message.c_str(); }

private:
    std::string m_message;
};

} // namespace e172

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define todo throw UnimplementedException(__FUNCSIG__)
#else
#define todo throw UnimplementedException(__PRETTY_FUNCTION__)
#endif
