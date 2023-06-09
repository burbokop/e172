#pragma once

#include "../utility/buffer.h"
#include <functional>
#include <type_traits>

namespace e172 {

template<typename T>
concept Length = requires(const T v) {
    {
        v.length()
    } -> std::convertible_to<double>;
};

template<typename T>
concept MathModule = requires(const T v) {
    {
        v.module()
    } -> std::convertible_to<double>;
};

double eFunction(double x, double c);

template<typename T>
class Kinematics {
public:
    Kinematics() = default;

    void addAcceleration(T value) { m_acceleration += value; }

    void addLimitedAcceleration(T value, double maxVelocity)
        requires std::is_arithmetic<T>::value || Length<T> || MathModule<T>
    {
        if constexpr (std::is_arithmetic<T>::value) {
            addAcceleration(eFunction(m_velocity, maxVelocity) * value);
        } else if constexpr (Length<T>) {
            addAcceleration(eFunction(m_velocity.length(), maxVelocity) * value);
        } else {
            addAcceleration(eFunction(m_velocity.module(), maxVelocity) * value);
        }
    }

    void addFriction(double coeficient);
    void addLimitedFriction(double maxVelocity, double coeficient);
    void accept(double deltaTime);
    T value() const { return m_value; }
    T velocity() const { return m_velocity; }
    T acceleration() const { return m_acceleration; }

    std::function<T(const T &)> valueProcessor() const { return m_valueProcessor; }
    void setValueProcessor(const std::function<T(const T &)> &valueProcessor)
    {
        m_valueProcessor = valueProcessor;
    }

    void setVelocity(const T &value) { m_velocity = value; }
    void setValue(const T &value) { m_value = value; }

    void serialize(WriteBuffer &buf) const
        requires Serialize<T>
    {
        buf.write(m_value);
        buf.write(m_velocity);
        buf.write(m_acceleration);
    }

    bool deserializeAssign(ReadBuffer &buf)
        requires Deserialize<T>
    {
        e172_chainingAssignOrElse(m_value, buf.read<T>(), false);
        e172_chainingAssignOrElse(m_velocity, buf.read<T>(), false);
        e172_chainingAssignOrElse(m_acceleration, buf.read<T>(), false);
        return true;
    }

private:
    template<typename V>
    static constexpr V initValue()
    {
        if constexpr (std::is_integral<V>::value) {
            return 0;
        } else {
            return T();
        }
    }

    T m_value = initValue<T>();
    T m_velocity = initValue<T>();
    T m_acceleration = initValue<T>();

    std::function<T(const T &)> m_valueProcessor;
};

template<typename T>
void Kinematics<T>::addFriction(double coeficient) {
    addAcceleration(m_velocity * coeficient * (-1));
}

template<typename T>
void Kinematics<T>::addLimitedFriction(double maxVelocity, double coeficient) {
    addLimitedAcceleration(m_velocity * coeficient * -1, maxVelocity);
}

template<typename T>
void Kinematics<T>::accept(double deltaTime) {
    m_velocity += m_acceleration * deltaTime;
    if(m_valueProcessor) {
        m_value = m_valueProcessor(m_value + (m_velocity * deltaTime));
    } else {
        m_value += m_velocity * deltaTime;
    }
    m_acceleration = initValue<T>();
}


}
