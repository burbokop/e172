// Copyright 2023 Borys Boiko

#pragma once

#include <list>
#include <algorithm>

namespace e172 {

template <typename T>
class CyclicList {
public:
    CyclicList() = default;

    operator const std::list<T> &() const { return m_container; }

    auto begin() const { return m_container.begin(); }
    auto begin() { return m_container.begin(); }
    auto end() const { return m_container.end(); }
    auto end() { return m_container.end(); }
    auto size() const { return m_container.size(); }

    void nextCycle() {
        if (m_autoIt == m_container.end()) {
            m_autoIt = m_container.begin();
            return;
        }

        if (++m_autoIt == m_container.end()) {
            m_autoIt = m_container.begin();
        }
    }

    T cyclicValue(const T& defaultValue = T()) const {
        return m_autoIt == m_container.end() ? defaultValue : *m_autoIt;
    };

    template<typename It>
    auto erase(It it) {
        bool do_reset = m_autoIt == it;
        it = m_container.erase(it);
        if (do_reset) {
            m_autoIt = it;
        }
        return it;
    }

    template<typename Value>
    void remove(const Value &value) {
        const auto it = std::find(m_container.begin(), m_container.end(), value);
        if (it != m_container.end())
            erase(it);
    }

    void push_back(const T &value) { m_container.push_back(value); }

private:
    std::list<T> m_container;
    typename std::list<T>::iterator m_autoIt = m_container.begin();
};

} // namespace e172
