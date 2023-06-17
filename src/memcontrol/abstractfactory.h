// Copyright 2023 Borys Boiko

#pragma once

#include "../type.h"
#include "src/meta.h"
#include <functional>
#include <list>
#include <map>
#include <optional>

namespace e172 {

/**
 * @brief The AbstractFactory class provides standart abstract factory
 * @details Template parameter <b>BaseClass</b> defines witch type is handled by specific instance of factory.
 * Factory handle only <b>BaseClass</b> type and types inherited by <b>BaseClass</b>.
 * Factory can not work with interfaces because them can not be created, but base type can be interface.
 * Template parameter <b>KeyType</b> defines witch type to use for store type names (is you use QString, std::string, or other type witch is casted from <b>const char*</b> typeName is set automaticaly by using KType class. Otherwise you mast set is manualy.)
 *
 * @note AbstractFactory is available whithout Qt.
 */
template<typename KeyType, typename BaseClass>
class AbstractFactory
{
public:
    using Creater = std::function<BaseClass *()>;
    using CreaterMap = std::map<KeyType, Creater>;

    AbstractFactory() = default;

    /**
     * @brief create function creates object with type name <b>typeName</b>.
     * @param typeName
     * @return pointer to new object
     */
    BaseClass *create(const KeyType &typeName)
    {
        const auto it = m_creaters.find(typeName);
        if (it == m_creaters.end()) {
            return nullptr;
        }
        auto obj = it->second();
        m_objects[obj] = typeName;
        return obj;
    }

    /**
     * @brief type function provide getting type name from object pointer.
     * @param object
     * @return type name or std::nullopt if object
     */
    std::optional<KeyType> type(BaseClass *object) const
    {
        auto it = m_objects.find(object);
        if (it != m_objects.end())
            return it->second;
        return std::nullopt;
    }

    /**
     * @brief registerType function provide registering type <b>T</b>.
     * @details After calling this function current instance of factory can create instances of <b>T</b>.
     * @param typeName
     * @return type name passed as paremeter without changes
     */
    template<typename T>
    KeyType registerType(const KeyType &typeName)
        requires std::is_base_of<BaseClass, T>::value
    {
        m_creaters[typeName] = []() { return new T(); };
        return typeName;
    }

    /**
     * @brief registerType overload provide registering type <b>T</b> but use KType for setting type name.
     * @return type name
     */
    template<typename T>
    KeyType registerType()
        requires std::is_base_of<BaseClass, T>::value
    {
        if constexpr (MetaType<T>) {
            const auto typeName = Meta::fromType<T>().typeName();
            m_creaters[typeName] = []() { return FactoryMeta::make<T>(); };
            return typeName;
        } else {
            return registerType<T>(Type<T>::name());
        }
    }

    std::list<KeyType> typeNames() const
    {
        std::list<KeyType> result;
        for (const auto &c : m_creaters) {
            result.push_back(c.first);
        }
        return result;
    }

    template<typename T>
        requires std::is_base_of<BaseClass, T>::value
    const KeyType &registerType(const KeyType &typeName, const Creater &creater)
    {
        m_creaters[typeName] = creater;
        return typeName;
    }

    /**
     * @brief clear factory
     */
    void clear() {
        m_creaters.clear();
        m_objects.clear();
    }

private:
    CreaterMap m_creaters;
    std::map<BaseClass *, KeyType> m_objects;
};

} // namespace e172
