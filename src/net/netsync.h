// Copyright 2023 Borys Boiko

#pragma once

#include <concepts>
#include <src/entity.h>
#include <src/utility/buffer.h>

namespace e172 {

/**
 * @brief The AbstractNetSync class is a abstraction under value containing in e172::Entity derived type as field and suppoused to be synchonized with network
 */
class AbstractNetSync
{
    friend Entity;

public:
    /**
     * @brief AbstractNetSync
     * @param e - parent entity
     */
    AbstractNetSync(Entity &e) { e.installNetSync(this); };
    virtual ~AbstractNetSync() = default;

    /**
     * @brief dirty
     * @return true if value need to be synchronized
     */
    virtual bool dirty() const = 0;

protected:
    /**
     * @brief serialize - write value to buffer
     */
    virtual void serialize(WriteBuffer &) const = 0;

    /**
     * @brief deserialize - read from buffer
     * @return true if succesfully deserialized
     */
    virtual bool deserialize(ReadBuffer &) = 0;

    /**
     * @brief wash - mark synchronized
     */
    virtual void wash() = 0;
};

/**
 * @brief The NetSync class - wraps T to be auto synchronized with network
 */
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
