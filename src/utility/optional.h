#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <ostream>
#include <functional>

namespace e172 {

enum NoneType { None };

template<typename T>
class Optional {
    bool m_isDefined;
    T m_value;
public:
    Optional(const T& value) {
        m_value = value;
        m_isDefined = true;
    }
    Optional(NoneType) {
        m_isDefined = false;
    }

    T value() const {
        return m_value;
    }

    bool isDefined() const {
        return m_isDefined;
    }
    bool isEmpty() const {
        return !m_isDefined;
    }

    template<typename R>
    R fold(const std::function<R(const T&)>& onOk, const std::function<R()>& onNull = [](){ return R(); }) const {
        if(m_isDefined) {
            return onOk(m_value);
        } else {
            return onNull();
        }
    }

    void fold(const std::function<void(const T&)>& onOk, const std::function<void()>& onNull = [](){}) const {
        if(m_isDefined) {
            onOk(m_value);
        } else {
            onNull();
        }
    }

    friend std::ostream &operator<<(std::ostream &stream, const Optional<T>& opt) {
        if(opt.m_isDefined) {
            return stream << opt.m_value;
        } else {
            return stream << "none";
        }
    }


    friend bool operator==(const Optional& v0, const T& v1) {
        return v0.m_isDefined ? v0.m_value == v1 : false;
    }
    friend bool operator!=(const Optional& v0, const T& v1) {
        return v0.m_isDefined ? v0.m_value != v1 : true;
    }
    friend bool operator==(const T& v1, const Optional& v0) {
        return v0.m_isDefined ? v0.m_value == v1 : false;
    }
    friend bool operator!=(const T& v1, const Optional& v0) {
        return v0.m_isDefined ? v0.m_value != v1 : true;
    }


    friend bool operator==(const Optional& v0, NoneType) {
        return !v0.m_isDefined;
    }
    friend bool operator!=(const Optional& v0, NoneType) {
        return v0.m_isDefined;
    }
    friend bool operator==(NoneType, const Optional& v0) {
        return !v0.m_isDefined;
    }
    friend bool operator!=(NoneType, const Optional& v0) {
        return v0.m_isDefined;
    }


    friend bool operator==(const Optional& v0, const Optional& v1) {
        return v0.m_isDefined && v1.m_isDefined ? v0.m_value == v1.m_value : v0.m_isDefined == v1.m_isDefined;
    }
    friend bool operator!=(const Optional& v0, const Optional& v1) {
        return v0.m_isDefined && v1.m_isDefined ? v0.m_value != v1.m_value : v0.m_isDefined != v1.m_isDefined;
    }
};

}
#endif // OPTIONAL_H
