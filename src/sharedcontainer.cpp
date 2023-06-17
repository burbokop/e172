// Copyright 2023 Borys Boiko

#include "sharedcontainer.h"
#include <stdexcept>

namespace e172 {

void SharedContainer::detach()
{
    if (m_refCountPtr) {
        --(*m_refCountPtr);
        if (*m_refCountPtr <= 0) {
            if (!m_destructor && m_data) {
                throw std::runtime_error("[e172::SharedContainer]: destructor is not set\n");
            }

            m_destructor(m_data);
            delete m_refCountPtr;
            m_refCountPtr = nullptr;
        }
    } else if (m_data) {
        throw std::runtime_error("[e172::SharedContainer]: detaching broken object\n");
    }
}

SharedContainer::SharedContainer(const SharedContainer &obj) {
    operator=(obj);
}

void SharedContainer::operator=(const SharedContainer &obj) {
    detach();

    m_destructor = obj.m_destructor;
    m_data = obj.m_data;
    m_refCountPtr = obj.m_refCountPtr;
    m_provider = obj.m_provider;

    if (m_refCountPtr)
        ++(*m_refCountPtr);
}

} // namespace e172
