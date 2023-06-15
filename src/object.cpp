#include "object.h"

#include <cstdint>

bool e172::Object::liveInHeap() const {
    return m_liveInHeap;
}

bool e172::Object::liveInSharedPtr() const
{
    try {
        return const_cast<Object *>(this)->shared_from_this() != std::shared_ptr<Object>();
    } catch (std::bad_weak_ptr) {
        return false;
    }
}

int *heap_example = new int();

e172::Object::Object() {
    int stack_example;
    if(((void*)this < &stack_example) == ((void*)heap_example < (void*)&stack_example)) {
        m_liveInHeap = true;
    } else {
        m_liveInHeap = false;
    }
}

template<e172::Weak AAA>
void aaaaa(AAA)
{}

class B : public e172::Object
{
public:
    B() = default;
};

template<e172::Weak AAA>
struct OOO
{
    AAA a;
};

e172::Object::LifeInfo e172::Object::lifeInfo() const
{
    e172::Object o;
    B b;
    aaaaa(o);
    aaaaa(b);

    const auto o0 = OOO<Object>{.a = o};
    const auto o1 = OOO<B>{.a = b};

    LifeInfo lifeInfo;
    lifeInfo.m_data = m_lifeInfoData;
    return lifeInfo;
}

e172::Object::~Object() {
    *m_lifeInfoData = false;
}
