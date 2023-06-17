// Copyright 2023 Borys Boiko

#pragma once

#include "loadable.h"
#include "loadabletemplate.h"
#include <map>
#include <memory>
#include <src/memcontrol/abstractfactory.h>
#include <src/utility/ptr.h>
#include <string>
#include <vector>

namespace e172 {

class AbstractAssetExecutor;
class AbstractGraphicsProvider;
class AbstractAudioProvider;
class Context;
class GameApplication;

class AssetProvider : public std::enable_shared_from_this<AssetProvider>
{
    friend GameApplication;
    friend AbstractAssetExecutor;
public:
    AssetProvider() = default;

    Loadable *createLoadable(const LoadableTemplate &loadableTemplate);
    Loadable *createLoadable(const std::string& templateId);
    template<typename T>
    auto createLoadable(const LoadableTemplate& loadableTemplate) {
        return e172::smart_cast<T>(createLoadable(loadableTemplate));
    }
    template<typename T>
    auto createLoadable(std::string templateId) {
        return e172::smart_cast<T>(createLoadable(templateId));
    }

    void searchInFolder(std::string path);
    std::vector<std::string> loadableNames();

    void addTemplate(const LoadableTemplate& tmpl);

    template<typename T>
    void registerType()
        requires std::is_base_of<Loadable, T>::value
    {
        m_factory.registerType<T>();
    }

    void installExecutor(const std::string &id,
                         const std::shared_ptr<AbstractAssetExecutor> &executor);

private:
    void processFile(std::string file, std::string path);
    LoadableTemplate createTemplate(const e172::VariantMap &root, const std::string &path);

private:
    std::shared_ptr<AbstractGraphicsProvider> m_graphicsProvider;
    std::shared_ptr<AbstractAudioProvider> m_audioProvider;
    Context *m_context = nullptr;
    AbstractFactory<std::string, Loadable> m_factory;
    std::map<std::string, LoadableTemplate> m_templates;
    std::map<std::string, std::shared_ptr<AbstractAssetExecutor>> m_executors;
};

} // namespace e172
