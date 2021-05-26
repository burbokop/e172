#ifndef PTR_H
#define PTR_H

#include <src/object.h>
#include <src/sfinae.h>
#include <functional>
namespace e172 {


template <typename T>
class ptr {
    template<typename A> friend class ptr;
    T *m_data = nullptr;
    Object::LifeInfo m_lifeInfo;
public:
    E172_SFINAE_METHOD_CHECKER(lifeInfo)
    ptr() {}
    ptr(const std::nullptr_t&) {}
    template<typename ...Args>
    static ptr<T> make(Args ...args) { return new T(args...); }

    template<typename TT>
    ptr(TT* data) {
        m_data = data;
        static_assert (has_lifeInfo_method<TT>::value, "T must be inherited by e172::Object");
        if constexpr(has_lifeInfo_method<TT>::value) {
            if(m_data)
                m_lifeInfo = m_data->lifeInfo();
        }
    }
    template<typename O>
    ptr(const ptr<O>& p) { operator=(p); }
    template<typename O>
    void operator=(const ptr<O> &p) {
        m_data = p.m_data;
        m_lifeInfo = p.m_lifeInfo;
    }
    T *operator->() { return m_data; };
    T *operator->() const { return m_data; };
    T* data() const { return m_data; }

    operator bool() const { return m_lifeInfo; }    

    template<typename R>
    R fold(const std::function<R(T*)>& onOk, const std::function<R()>& onNull = [](){ return R(); }) const {
        if(operator bool()) {
            return onOk(data());
        } else {
            return onNull();
        }
    }

    void fold(const std::function<void(T*)>& onOk, const std::function<void()>& onNull = [](){}) const {
        if(operator bool()) {
            onOk(data());
        } else {
            onNull();
        }
    }

    bool safeDestroy() const {
        if(operator bool()) {
            if(m_data->liveInHeap()) {
                delete m_data;
                return true;
            }
        }
        return false;
    }
};

template<typename T>
struct smart_ptr_type {
    typedef typename std::remove_pointer<T>::type no_ptr_t;
    typedef typename ptr<no_ptr_t>::template has_lifeInfo_method<no_ptr_t> is_object;
    typedef typename std::conditional<is_object::value, ptr<no_ptr_t>, no_ptr_t*>::type type;
};


template <typename T>
bool safeDestroy(const e172::ptr<T> &ptr) {
    return ptr.safeDestroy();
}

template<typename T, typename A>
auto smart_cast(const ptr<A> &p) {
    typedef typename smart_ptr_type<T>::type return_type;
    if(p) {
        return return_type(p->template cast<T>());
    }
    return return_type();
}

template<typename T, typename A>
auto smart_cast(A *p) {
    typedef typename std::remove_pointer<T>::type no_ptr_t;
    typedef typename smart_ptr_type<T>::type return_type;
    if(p) {
        return return_type(dynamic_cast<no_ptr_t*>(p));
    }
    return return_type();
}


}

template<typename A, typename B>
bool operator ==(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1) {
    return ptr0.data() == ptr1.data();
}
template<typename A, typename B>
bool operator !=(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1) {
    return ptr0.data() != ptr1.data();
}
template<typename A, typename B>
bool operator ==(const e172::ptr<A> &ptr0, B *ptr1) {
    return ptr0.data() == ptr1;
}
template<typename A, typename B>
bool operator !=(const e172::ptr<A> &ptr0, B *ptr1) {
    return ptr0.data() != ptr1;
}
template<typename A, typename B>
bool operator ==(A *ptr0, const e172::ptr<B> &ptr1) {
    return ptr0 == ptr1.data();
}
template<typename A, typename B>
bool operator !=(A *ptr0, const e172::ptr<B> &ptr1) {
    return ptr0 != ptr1.data();
}

template<typename A, typename B>
bool operator <(const e172::ptr<A> &ptr0, const e172::ptr<B> &ptr1) {
    return ptr0.data() < ptr1.data();
}

template<typename T>
std::ostream &operator<<(std::ostream& stream, const e172::ptr<T>& ptr) {
    if(ptr.data()) {
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

template<typename A>
bool operator ==(const e172::ptr<A> &ptr, std::nullptr_t) {
    return !ptr;
}
template<typename A>
bool operator !=(const e172::ptr<A> &ptr, std::nullptr_t) {
    return ptr;
}
template<typename A>
bool operator ==(std::nullptr_t, const e172::ptr<A> &ptr) {
    return !ptr;
}
template<typename A>
bool operator !=(std::nullptr_t, const e172::ptr<A> &ptr) {
    return ptr;
}


#endif // PTR_H
