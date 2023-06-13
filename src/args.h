#ifndef ARGS_H
#define ARGS_H

#include "variant.h"


namespace e172 {

class Args {
    VariantVector m_v;
public:
    template<typename ...T>
    Args(const T& ...args) : m_v(VariantVector { args... }) {}

    operator Variant() const { return m_v; }
    operator VariantVector() const { return m_v; }
    VariantVector value() const { return m_v; }

    template<typename T>
    friend Args &operator <<(Args& args, const T& v) {
        args.m_v.push_back(v);
        return args;
    }

};


}
#endif // ARGS_H
