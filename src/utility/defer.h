#ifndef DEFER_H
#define DEFER_H

#include <functional>

namespace e172 {

class Defer {
    std::function<void()> m_f;
public:
    Defer(const std::function<void()> &f);
    ~Defer();
};

}
#endif // DESTROYSIGNAL_H
