// Copyright 2023 Borys Boiko

#pragma once

#include "ptr.h"
#include <functional>
#include <list>
#include <map>
#include <utility>

namespace e172 {

template<typename T, typename P>
class Signal;

/**
 * @brief The Signal class - provides "one notifies, many subscribe" behaviour
 * @note P - some private class niided in place where you call operator(), but not neeeded where you connect or disconnect. This make operator() private
 *
 * Example:
 * 
 * class MyBd {
 *     struct Private {};
 * public:
 *     using Status = std::uint32_t;
 *     void sendRequest(const std::string& query) {
 *          ...
 *          m_requestTransmitted(status)
 *     }
 *
 *     auto &requestTransmitted() { return m_requestTransmitted; }
 * private:
 *     Signal<void(Status), Private> m_requestTransmitted;
 * }
 *
 * MyBd bd;
 * // will be called after sendRequest function called
 * bd.requestTransmitted().connect([](MyBd::Status status) { std::cout << status << std::endl; });
 * bd.sendRequest("...");
 *
 */
template<typename R, typename... Args, typename P>
class Signal<R(Args...), P>
{
public:
    using Func = std::function<R(Args...)>;
    using Handle = std::size_t;

    Signal() = default;
    Signal(const Signal &) = delete;
    Signal(Signal &&) = delete;

    auto operator=(const Signal &) = delete;
    auto operator=(Signal &&) = delete;

    /**
     * @brief operator () - invoke signal
     * @param args
     */
    void operator()(Args... args, P)
    {
        for (auto &f : m_f) {
            f.second(args...);
        }

        auto it = m_cf.begin();
        while (it != m_cf.end()) {
            if (it->second.first) {
                it->second.second(args...);
                ++it;
            } else {
                it = m_cf.erase(it);
            }
        }
    }

    Handle connect(const Func &f)
    {
        if (!f)
            return 0;

        m_f.insert({m_nextHandle, f});
        return m_nextHandle++;
    }

    Handle connect(const Object *ctx, const Func &f)
    {
        if (!f || !ctx)
            return 0;

        m_cf.insert({m_nextHandle, {ctx, f}});
        return m_nextHandle++;
    }

    bool disconnect(Handle handle)
    {
        if (m_f.erase(handle)) {
            return true;
        }
        if (m_cf.erase(handle)) {
            return true;
        }
        return false;
    }

private:
    std::size_t m_nextHandle = 1;
    std::map<Handle, Func> m_f;
    std::map<Handle, std::pair<ptr<const Object>, Func>> m_cf;
};

} // namespace e172
