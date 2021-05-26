#ifndef ASSETPROVIDER_H
#define ASSETPROVIDER_H

#include <src/memcontrol/abstractfactory.h>
#include <memory>
#include "loadable.h"
#include "loadabletemplate.h"
#include <src/utility/ptr.h>

namespace e172 {

class AbstractAssetExecutor;
class AbstractGraphicsProvider;
class AbstractAudioProvider;
class Context;

class AssetProvider {
    friend class GameApplication;
    friend AbstractAssetExecutor;
    AbstractGraphicsProvider *m_graphicsProvider = nullptr;
    AbstractAudioProvider *m_audioProvider = nullptr;
    Context *m_context = nullptr;

    AbstractFactory<std::string, Loadable> m_factory;
    std::map<std::string, LoadableTemplate> templates;
    std::map<std::string, std::shared_ptr<AbstractAssetExecutor>> executors;

    void processFile(std::string file, std::string path);

    LoadableTemplate createTemplate(const e172::VariantMap& root, const std::string &path);
public:
    Loadable *createLoadable(const LoadableTemplate& loadableTemplate);
    Loadable *createLoadable(const std::string& templateId);
    template<typename T>
    auto createLoadable(const LoadableTemplate& loadableTemplate) {
        return e172::smart_cast<T>(createLoadable(loadableTemplate));
    }
    template<typename T>
    auto createLoadable(std::string templateId) {
        return e172::smart_cast<T>(createLoadable(templateId));
    }



    AssetProvider();
    void searchInFolder(std::string path);
    std::vector<std::string> loadableNames();

    void addTemplate(const LoadableTemplate& tmpl);

    template<typename T>
    void registerType() {
        m_factory.registerType<T>();
    }

    void installExecutor(const std::string &id, const std::shared_ptr<AbstractAssetExecutor> &executor);
};

}

#endif // ASSETPROVIDER_H
