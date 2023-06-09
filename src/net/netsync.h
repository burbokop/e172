#pragma once

#include <concepts>
#include <src/entity.h>
#include <src/utility/buffer.h>

namespace e172 {

class AbstractNetSync
{
    friend Entity;

public:
    AbstractNetSync(Entity &e) { e.installNetSync(this); };
    ~AbstractNetSync() = default;

    virtual bool dirty() const = 0;

protected:
    virtual void serialize(WriteBuffer &) const = 0;
    virtual bool deserialize(ReadBuffer &) = 0;
    virtual void wash() = 0;
};

template<typename T>
requires Serialize<T> && Deserialize<T>
    class NetSync : public AbstractNetSync
{
public:
    NetSync(const T &val, Entity &e)
        : AbstractNetSync(e)
        , m_value(val)
    {
    }

    NetSync<T> &operator=(const T &val)
    {
        m_value = val;
        m_dirty = true;
        return *this;
    }

    operator T() const { return m_value; }

    // AbstractNetSync interface
public:
    bool dirty() const override { return m_dirty; }

protected:
    void serialize(WriteBuffer &buffer) const override { buffer.write(m_value); }

    bool deserialize(ReadBuffer &b) override
    {
        if (const auto v = b.read<T>()) {
            m_value = *v;
            return true;
        } else {
            return false;
        }
    }

    void wash() override { m_dirty = false; };

private:
    T m_value;
    bool m_dirty = true;
};

} // namespace e172
