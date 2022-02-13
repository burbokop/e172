#include "defer.h"


e172::Defer::Defer(const std::function<void ()> &f) {
    m_f = f;
}

e172::Defer::~Defer() {
    if(m_f)
        m_f();
}
