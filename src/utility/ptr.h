// Copyright 2023 Borys Boiko

#pragma once

#include "../object.h"
#include "../sfinae.h"
#include <functional>
#include <memory>
#include <set>

namespace e172 {

template<typename T>
class ptr
{
    template<typename A>
    friend class ptr;

public:
    E172_SFINAE_METHOD_CHECKER(lifeInfo)
    ptr() {}
    ptr(const std::nullptr_t&) {}
    template<typename ...Args>
    static ptr<T> make(Args ...args) { return new T(args...); }

    template<typename TT>
    ptr(TT *data)
    {
        m_data = data;
        static_assert(has_lifeInfo_method<TT>::value, "T must be inherited by e172::Object");
        if constexpr (has_lifeInfo_method<TT>::value) {
            if (m_data)
                m_lifeInfo = m_data->lifeInfo();
        }
    }

    template<typename O>
    ptr(const ptr<O> &p)
        requires(std::is_const<T>::value || !std::is_const<O>::value)
    {
        operator=(p);
    }

    template<typename TT>
    ptr(const std::weak_ptr<TT> &w)
        : ptr(w.lock().get())
    {}

    template<typename TT>
    ptr(const std::shared_ptr<TT> &s)
        : ptr(s.get())
    {}

    template<typename O>
    void operator=(const ptr<O> &p)
        requires(std::is_const<T>::value || !std::is_const<O>::value)
    {
        m_data = p.m_data;
        m_lifeInfo = p.m_lifeInfo;
    }

    T *operator->() { return m_data; };
    T *operator->() const { return m_data; };
    T *data() const { return m_data; }

    operator bool() const { return m_lifeInfo; }

    template<typename R>
    R fold(
        const std::function<R(T *)> &onOk,
        const std::function<R()> &onNull = []() { return R(); }) const
    {
        if (operator bool()) {
            return onOk(data());
        } else {
            return onNull();
        }
    }

    void fold(
        const std::function<void(T *)> &onOk, const std::function<void()> &onNull = []() {}) const
    {
        if (operator bool()) {
            onOk(data());
        } else {
            onNull();
        }
    }

    /**
     * @brief destroy
     * @return false if already destroyed
     */
    bool destroy() const
    {
        if (operator bool()) {
            if (m_data->liveInHeap() && !m_data->liveInSharedPtr()) {
                delete m_data;
                return true;
            }
        }
        return false;
    }

private:
    T *m_data = nullptr;
    Object::LifeInfo m_lifeInfo;
};

template<typename T>
struct smart_ptr_type {
    typedef typename std::remove_pointer<T>::type no_ptr_t;
    typedef typename ptr<no_ptr_t>::template has_lifeInfo_method<no_ptr_t> is_object;
    typedef typename std::conditional<is_object::value, ptr<no_ptr_t>, no_ptr_t *>::type type;
};

/**
 * @brief destroy - safe destroy ptr<T>
 * @param ptr
 * @return false if already destroyed
 */
template<typename T>
bool destroy(const e172::ptr<T> &ptr)
{
    return ptr.destroy();
}

template<typename T, typename A>
auto smart_cast(const ptr<A> &p) {
    typedef typename smart_ptr_type<T>::type return_type;
    if (p) {
        return return_type(p->template cast<T>());
    }
    return return_type();
}

template<typename T, typename A>
auto smart_cast(A *p) {
    typedef typename std::remove_pointer<T>::type no_ptr_t;
    typedef typename smart_ptr_type<T>::type return_type;
    if (p) {
        return return_type(dynamic_cast<no_ptr_t*>(p));
    }
    return return_type();
}

template<typename A, typename B>
bool operator==(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1)
{
    return ptr0.data() == ptr1.data();
}

template<typename A, typename B>
bool operator!=(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1)
{
    return ptr0.data() != ptr1.data();
}

template<typename A, typename B>
bool operator==(const e172::ptr<A> &ptr0, B *ptr1)
{
    return ptr0.data() == ptr1;
}

template<typename A, typename B>
bool operator!=(const e172::ptr<A> &ptr0, B *ptr1)
{
    return ptr0.data() != ptr1;
}

template<typename A, typename B>
bool operator==(A *ptr0, const e172::ptr<B> &ptr1)
{
    return ptr0 == ptr1.data();
}

template<typename A, typename B>
bool operator!=(A *ptr0, const e172::ptr<B> &ptr1)
{
    return ptr0 != ptr1.data();
}

template<typename A, typename B>
bool operator<(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1)
{
    return ptr0.data() < ptr1.data();
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, const e172::ptr<T> &ptr)
{
    if (ptr.data()) {
        if (ptr.operator bool()) {
            stream << ptr.data();
        } else {
            stream << ptr.data() << "[deleted]";
        }
    } else {
        stream << ptr.data();
    }
    return stream;
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::set<e172::ptr<T>> &set)
{
    size_t i = 0;
    stream << "[";
    for (const auto &ptr : set) {
        stream << ptr;
        if (i < set.size() - 1)
            stream << ", ";
        ++i;
    }
    return stream << "]";
}

} // namespace e172
