// Copyright 2023 Borys Boiko

#pragma once

#include "../traits.h"
#include "../utility/ptr.h"
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <variant>

namespace e172 {

template<typename T, typename P>
class Callback;

template<typename R, typename... Args, typename P>
class Callback<R(Args...), P>
{
public:
    using Func = std::function<R(Args...)>;
    using Handle = std::size_t;

    Callback() = default;
    Callback(const Callback &) = delete;
    Callback(Callback &&) = delete;

    auto operator=(const Callback &) = delete;
    auto operator=(Callback &&) = delete;

    /**
     * @brief operator () - invoke callback
     * @param args
     */
    std::optional<R> operator()(Args &&...args, P)
        requires(!std::is_same<R, void>::value)
    {
        return std::visit(e172::Overloaded{
                              [&](const Func &f) -> std::optional<R> {
                                  return f ? f(std::move(args)...) : std::nullopt;
                              },
                              [&](const ContextedFunc &f) -> std::optional<R> {
                                  return (f.first && f.second) ? f.second(std::move(args)...)
                                                               : std::nullopt;
                              },
                          },
                          m_f);
    }

    /**
     * @brief operator () - invoke callback
     * @param args
     */
    void operator()(Args &&...args, P)
        requires std::is_same<R, void>::value
    {
        std::visit(e172::Overloaded{
                       [&](const Func &f) { f ? f(std::move(args)...) : void(); },
                       [&](const ContextedFunc &f) {
                           (f.first && f.second) ? f.second(std::move(args)...) : void();
                       },
                   },
                   m_f);
    }

    void connect(const Func &f) { m_f = f; }

    void connect(const Object *ctx, const Func &f) { m_f = ContextedFunc{ctx, f}; }

    bool disconnect()
    {
        if (std::visit(e172::Overloaded{
                           [](const Func &f) -> bool { return f; },
                           [](const ContextedFunc &f) -> bool { return f.first && f.second; },
                       },
                       m_f)) {
            m_f = nullptr;
            return true;
        }
        return false;
    }

private:
    using ContextedFunc = std::pair<ptr<const Object>, Func>;

private:
    std::variant<Func, ContextedFunc> m_f;
};

} // namespace e172
