#pragma once

#include <string>

namespace e172 {

class Informative
{
public:
    virtual std::string info() const = 0;
    virtual ~Informative();
};

} // namespace e172
