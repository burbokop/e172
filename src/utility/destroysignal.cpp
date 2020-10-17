#include "destroysignal.h"



e172::DestroySignal::DestroySignal(const std::function<void ()> &f) {
    m_f = f;
}

e172::DestroySignal::~DestroySignal() {
    if(m_f)
        m_f();
}
