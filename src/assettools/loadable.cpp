// Copyright 2023 Borys Boiko

#include "loadable.h"

namespace e172 {

void Loadable::registerInitFunction(const std::function<void()> &function)
{
    if (!m_released) {
        m_initFunctions.push_back(function);
    }
}

} // namespace e172
