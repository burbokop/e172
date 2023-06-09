#pragma once

#include "type.h"
#include <memory>

namespace e172 {

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

    const std::string &typeName() const { return m_data->typeName; };
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

private:
    const Data *m_data = nullptr;
};

class FactoryMeta
{
public:
    FactoryMeta(const FactoryMeta &) = delete;
    FactoryMeta(FactoryMeta &&) = default;

    template<typename T, typename... Args>
    static T *make(Args... args)
    {
        return new T(FactoryMeta(Meta::fromType<T>()), args...);
    }

    template<typename T, typename... Args>
    static std::shared_ptr<T> makeShared(Args... args)
    {
        return std::make_shared<T>(FactoryMeta(Meta::fromType<T>()), args...);
    }

    template<typename T, typename... Args>
    static std::unique_ptr<T> makeUniq(Args... args)
    {
        return new T(FactoryMeta(Meta::fromType<T>()), args...);
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
