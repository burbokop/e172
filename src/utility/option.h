// Copyright 2023 Borys Boiko

#pragma once

#include <functional>
#include <ostream>
#include <sstream>
#include <string>

namespace e172 {

enum NoneType { None };

template<typename T>
class Option {
public:
    Option(const T& value) {
        m_value = value;
        m_isDefined = true;
    }
    Option(NoneType) { m_isDefined = false; }
    Option() { m_isDefined = false; }

    T value() const { return m_value; }

    bool isDefined() const { return m_isDefined; }
    bool isEmpty() const { return !m_isDefined; }

    template<typename R>
    R fold(
        const std::function<R(const T &)> &onOk,
        const std::function<R()> &onNull = []() { return R(); }) const
    {
        if (m_isDefined) {
            return onOk(m_value);
        } else {
            return onNull();
        }
    }

    void fold(
        const std::function<void(const T &)> &onOk,
        const std::function<void()> &onNull = []() {}) const
    {
        if (m_isDefined) {
            onOk(m_value);
        } else {
            onNull();
        }
    }

    template<typename R>
    e172::Option<R> map(const std::function<R(const T &)> &f)
    {
        if (m_isDefined) {
            return f(m_value);
        }
        return e172::None;
    }

    e172::Option<T> &modify(const std::function<void(T &)> &f)
    {
        if (m_isDefined) {
            f(m_value);
        }
        return *this;
    }

    operator std::string() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    std::string toString() const { return *this; }

    friend std::ostream &operator<<(std::ostream &stream, const Option<T> &opt)
    {
        if (opt.m_isDefined) {
            return stream << opt.m_value;
        } else {
            return stream << "none";
        }
    }

    friend bool operator==(const Option &v0, const T &v1)
    {
        return v0.m_isDefined ? v0.m_value == v1 : false;
    }

    friend bool operator!=(const Option &v0, const T &v1)
    {
        return v0.m_isDefined ? v0.m_value != v1 : true;
    }

    friend bool operator==(const T &v1, const Option &v0)
    {
        return v0.m_isDefined ? v0.m_value == v1 : false;
    }

    friend bool operator!=(const T &v1, const Option &v0)
    {
        return v0.m_isDefined ? v0.m_value != v1 : true;
    }

    friend bool operator==(const Option &v0, NoneType) { return !v0.m_isDefined; }
    friend bool operator!=(const Option &v0, NoneType) { return v0.m_isDefined; }
    friend bool operator==(NoneType, const Option &v0) { return !v0.m_isDefined; }
    friend bool operator!=(NoneType, const Option &v0) { return v0.m_isDefined; }

    friend bool operator==(const Option &v0, const Option &v1)
    {
        return v0.m_isDefined && v1.m_isDefined ? v0.m_value == v1.m_value : v0.m_isDefined == v1.m_isDefined;
    }

    friend bool operator!=(const Option &v0, const Option &v1)
    {
        return v0.m_isDefined && v1.m_isDefined ? v0.m_value != v1.m_value : v0.m_isDefined != v1.m_isDefined;
    }

private:
    bool m_isDefined;
    T m_value;
};
} // namespace e172
