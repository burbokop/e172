// Copyright 2023 Borys Boiko

#pragma once

#include "type.h"
#include <memory>
#include <string>
#include <utility>

namespace e172 {

class FactoryMeta;

template<class T, class... Args>
concept MetaType = std::constructible_from<T, FactoryMeta &&, Args...>;

/**
 * @brief The Meta class contains meta information about some other type
 */
class Meta
{
public:
    Meta(const Meta &) = default;
    Meta(Meta &&) = default;

    template<typename T>
    static Meta fromType()
    {
        return Meta(s_data<T>);
    }

    /**
     * @brief typeName
     * @return demangled type name with namespaces
     */
    const std::string &typeName() const { return m_data->typeName; };

    /**
     * @brief typeHash
     * @return type hash
     */
    size_t typeHash() const { return m_data->typeHash; };

    bool operator==(const Meta &rhs) const { return m_data == rhs.m_data; }
    std::strong_ordering operator<=>(const Meta &rhs) const { return m_data <=> rhs.m_data; }

private:
    struct Data
    {
        explicit Data(const std::string &typeName, size_t typeHash)
            : typeName(typeName)
            , typeHash(typeHash)
        {}

        Data(const Data &) = delete;
        Data(Data &&) = delete;

        const std::string typeName;
        const size_t typeHash;
    };

    template<typename T>
    inline static const Data *s_data = new Data(Type<T>::name(), Type<T>::hash());

    explicit Meta(const Data *data)
        : m_data(data)
    {}

    inline friend std::ostream &operator<<(std::ostream &stream, const Meta &meta)
    {
        if (meta.m_data) {
            return stream << "Meta(" << meta.m_data->typeName << ")";
        }
        return stream << "Meta(invalid)";
    }

private:
    const Data *m_data = nullptr;
};

/**
 * @brief The FactoryMeta class used to construct object injenting correct meta
 * Example:
 * ```
 * class MyClass {
 * public:
 *     MyClass(FactoryMeta&& m) : m_meta(m.meta()) {}
 * private:
 *     Meta m_meta;
 * };
 * ...
 * const auto mc = FactoryMeta::make<MyClass>();
 * assert(mc.meta().typeName() == "MyClass");
 * ```
 */
class FactoryMeta
{
public:
    FactoryMeta(const FactoryMeta &) = delete;
    FactoryMeta(FactoryMeta &&) = default;

    /**
     * @brief make - create object in heap
     * @param args
     * @return 
     */
    template<typename T, typename... Args>
    static T *make(Args &&...args)
    {
        return new T(FactoryMeta(Meta::fromType<T>()), std::forward<Args>(args)...);
    }

    /**
     * @brief makeShared - create object under shared pointer
     * @param args
     * @return 
     */
    template<typename T, typename... Args>
    static std::shared_ptr<T> makeShared(Args &&...args)
    {
        return std::make_shared<T>(FactoryMeta(Meta::fromType<T>()), std::forward<Args>(args)...);
    }

    /**
     * @brief makeUniq - create object under unique pointer
     * @param args - args to pass to constructor
     * @return
     */
    template<typename T, typename... Args>
    static std::unique_ptr<T> makeUniq(Args &&...args)
    {
        return std::make_unique<T>(FactoryMeta(Meta::fromType<T>()), std::forward<Args>(args)...);
    }

    const Meta &meta() const { return m_meta; };

private:
    explicit FactoryMeta(const Meta &meta)
        : m_meta(meta)
    {}

private:
    Meta m_meta;
};

} // namespace e172
