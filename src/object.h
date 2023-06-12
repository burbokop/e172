#pragma once

#include <type_traits>
#include <memory>

#define UNUSED(expr) do { (void)(expr); } while (0)

namespace e172 {

template<typename T, typename P>
inline P verbatim_cast(T value)
{
    return reinterpret_cast<P *>(&value)[0];
}

class Object : public std::enable_shared_from_this<Object>
{
public:
    Object();

    class LifeInfo {
        friend Object;
        std::shared_ptr<bool> m_data;
    public:
        inline LifeInfo() {};
        inline operator bool() const { return m_data ? *m_data : false; }
    };
    LifeInfo lifeInfo() const;

    template <typename Type>
    inline bool instanceOf() const { return cast<Type>(); }

    template <typename Type>
    inline bool instanceOf() { return cast<Type>(); }

    template <typename Type>
    inline typename std::remove_pointer<Type>::type *cast() const {
        typedef typename std::remove_pointer<Type>::type no_ptr_t;
        return dynamic_cast<const no_ptr_t*>(this);
    }

    template <typename Type>
    inline typename std::remove_pointer<Type>::type *cast() {
        typedef typename std::remove_pointer<Type>::type no_ptr_t;
        return dynamic_cast<no_ptr_t*>(this);
    }

    virtual ~Object();
    bool liveInHeap() const;
    bool liveInSharedPtr() const;

private:
    bool m_liveInHeap = false;
    std::shared_ptr<bool> m_lifeInfoData = std::make_shared<bool>(true);
};

} // namespace e172
