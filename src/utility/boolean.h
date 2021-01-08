#ifndef BOOLEAN_H
#define BOOLEAN_H

namespace e172 {

class boolean {
    bool m_value;
public:
    boolean(bool value = false) { m_value = value; }
    operator bool () const { return m_value; }
    bool value() const { return m_value; }
};

}

#endif // BOOLEAN_H
