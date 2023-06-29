// Copyright 2023 Borys Boiko

#pragma once

#include "../variant.h"
#include <map>
#include <string>
#include <vector>

namespace e172 {

class AssetProvider;

class Loadable {
    friend AssetProvider;

public:
    Loadable() = default;
    virtual ~Loadable() = default;

    template<typename T>
    T asset(const std::string &name, const T &defaultValue = T(), bool *ok = nullptr) const
    {
        const auto it = m_assets.find(name);
        if (it != m_assets.end() && it->second.containsType<T>()) {
            if (ok)
                *ok = true;
            return it->second.value<T>();
        }
        if (ok)
            *ok = false;
        return defaultValue;
    }

    const std::string &templateId() const { return m_templateId; }
    AssetProvider *assetProvider() { return m_assetProvider; }
    const AssetProvider *assetProvider() const { return m_assetProvider; }

protected:
    void registerInitFunction(const std::function<void(void)> &function);

    template<typename C>
    void registerInitFunction(C *obj, void (C::*f)())
    {
        registerInitFunction([obj, f]() { (obj->*f)(); });
    }

private:
    std::map<std::string, e172::Variant> m_assets;
    std::string m_loadableClassName;
    std::string m_templateId;
    AssetProvider *m_assetProvider = nullptr;
    std::vector<std::function<void(void)>> m_initFunctions;
    bool m_released = false;
};

} // namespace e172
