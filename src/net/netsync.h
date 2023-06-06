#pragma once

#include <concepts>
#include <src/entity.h>

namespace e172 {

template<typename T>
concept Serialize = requires(T const v)
{
    {
        v.serialize()
    } -> std::convertible_to<std::vector<std::uint8_t>>;
}
|| std::is_fundamental<T>::value;

template<typename T>
concept Deserialize = requires(std::vector<std::uint8_t> input)
{
    {
        T::deserialize(input)
    } -> std::convertible_to<T>;
}
|| std::is_fundamental<T>::value;

class AbstractNetSync
{
    friend Entity;

public:
    AbstractNetSync() = default;
    ~AbstractNetSync() = default;

    virtual bool dirty() const = 0;

protected:
    virtual Bytes serialize() const = 0;
    virtual void deserAssign(Bytes &&, bool markDirty) = 0;
    virtual void wash() = 0;
};

// clang-format off
template<typename T>
requires Serialize<T> && Deserialize<T>
    class NetSync : public AbstractNetSync
// clang-format on
{
public:
    NetSync(const T &val, Entity &e)
        : m_value(val)
    {
        e.installNetSync(this);
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
    Bytes serialize() const override
    {
        if constexpr (std::is_fundamental<T>::value) {
            return WriteBuffer::toBytes(m_value);
        } else {
            return m_value.serialize();
        }
    }

    void deserAssign(Bytes &&b, bool markDirty) override
    {
        if constexpr (std::is_fundamental<T>::value) {
            m_value = ReadBuffer::fromBytes<T>(std::move(b)).value();
        } else {
            m_value = T::deserialize(b);
        }
        if (markDirty) {
            m_dirty = true;
        }
    }

    void wash() override { m_dirty = false; };

private:
    T m_value;
    bool m_dirty = false;
};

} // namespace e172
