#ifndef DESTROYSIGNAL_H
#define DESTROYSIGNAL_H

#include <functional>

namespace e172 {

class DestroySignal {
    std::function<void()> m_f;
public:
    DestroySignal(const std::function<void()> &f);
    ~DestroySignal();
};

}
#endif // DESTROYSIGNAL_H
