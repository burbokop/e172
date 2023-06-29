// Copyright 2023 Borys Boiko

#pragma once

#include "../variant.h"
#include "loadabletemplate.h"
#include <memory>
#include <string>

namespace e172 {

class AbstractGraphicsProvider;
class AbstractAudioProvider;
class Loadable;
class AssetProvider;

class AbstractAssetExecutor {
    friend AssetProvider;

public:
    AbstractAssetExecutor() = default;

    std::string fullPath(const std::string &path);

    LoadableTemplate createTemplate(const e172::VariantMap& object);
    LoadableTemplate loadTemplate(const std::string &templateId);

    std::shared_ptr<AbstractGraphicsProvider> graphicsProvider() const
    {
        return m_graphicsProvider;
    }

    std::shared_ptr<AbstractAudioProvider> audioProvider() const { return m_audioProvider; }

    virtual Variant proceed(const Variant &value) = 0;
    virtual ~AbstractAssetExecutor() = default;

private:
    std::shared_ptr<AssetProvider> m_provider;
    std::shared_ptr<AbstractGraphicsProvider> m_graphicsProvider;
    std::shared_ptr<AbstractAudioProvider> m_audioProvider;
    std::string m_executorPath;
};

} // namespace e172
