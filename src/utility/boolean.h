// Copyright 2023 Borys Boiko

#pragma once

namespace e172 {

class Boolean
{
public:
    Boolean(bool value = false) { m_value = value; }
    operator bool () const { return m_value; }
    bool value() const { return m_value; }

private:
    bool m_value;
};

} // namespace e172
