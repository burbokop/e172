// Copyright 2023 Borys Boiko

#include "assetprovider.h"

#include <src/debug.h>
#include <src/graphics/abstractgraphicsprovider.h>
#include <src/additional.h>
#include <src/context.h>
#include "abstractassetexecutor.h"

namespace e172 {

void AssetProvider::searchInFolder(std::string path)
{
    if (path[path.length() - 1] == '/')
        path.pop_back();
    std::vector<std::string> items = Additional::directoryContent(path);
    for (std::size_t i = 0; i < items.size(); ++i) {
        std::string item = items[i];
        std::string file = path + '/' + item;
        if (Additional::isDirectory(file)) {
            searchInFolder(file);
        } else {
            processFile(file, path);
        }
    }
}

Loadable *AssetProvider::createLoadable(const std::string &templateId) {
    const auto it = m_templates.find(templateId);
    if (it == m_templates.end()) {
        Debug::warning("AssetProvider::createLoadable: Loadable template not found for id:", templateId);
        return nullptr;
    }
    return createLoadable(it->second);
}

std::vector<std::string> AssetProvider::loadableNames() {
    std::vector<std::string> result;
    for (auto it = m_templates.begin(); it != m_templates.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

void AssetProvider::addTemplate(const LoadableTemplate &tmpl) {
    m_templates[tmpl.id()] = tmpl;
}

void AssetProvider::installExecutor(const std::string &id, const std::shared_ptr<AbstractAssetExecutor> &executor) {
    m_executors[id] = executor;
}

void AssetProvider::processFile(std::string file, std::string path) {
    std::string sufix = Additional::fileSufix(file);
    if (sufix == ".json") {
        const auto root = Variant::fromJson(Additional::readFile(file)).toMap();
        if (root.size() == 0) {
            Debug::warning("Empty json object detected or parsing error. file:", file);
            return;
        }
        const auto t = createTemplate(root, path);
        if (t.isValid()) {
            addTemplate(t);
        }
    }
}

LoadableTemplate AssetProvider::createTemplate(const VariantMap &root, const std::string &path) {
    const auto id = Additional::value(root, "id");
    const auto className = Additional::value(root, "class");
    if (id.isNull() || className.isNull()) {
        if (id.isNull()) {
            Debug::warning("Template id missing. object:", root);
        } else {
            Debug::warning("Template class name missing. object:", root);
        }
        return LoadableTemplate();
    }

    e172::VariantMap resultAssets;
    for (auto item = root.begin(); item != root.end(); ++item) {
        const auto &assetId = item->first;
        if (assetId != "class" && assetId != "id") {
            if (item->second.isNull()) {
                Debug::warning("Asset is null. Id:", assetId, "object:", root);
            } else {
                auto it = m_executors.find(assetId);
                if (it != m_executors.end() && it->second) {
                    it->second->m_executorPath = m_context->absolutePath(path);
                    it->second->m_provider = shared_from_this();
                    it->second->m_graphicsProvider = m_graphicsProvider;
                    it->second->m_audioProvider = m_audioProvider;
                    resultAssets[assetId] = it->second->proceed(item->second);
                } else {
                    resultAssets[assetId] = item->second;
                }
            }
        }
    }
    return LoadableTemplate(id.toString(), className.toString(), resultAssets);
}

Loadable *AssetProvider::createLoadable(const LoadableTemplate &loadableTemplate) {
    auto result = m_factory.create(loadableTemplate.className());
    if (!result) {
        Debug::warning("AssetProvider::createLoadable: Type not registered:",
                       loadableTemplate.className(),
                       "( template id: ",
                       loadableTemplate.id(),
                       ")");
        return nullptr;
    }

    result->m_assets = loadableTemplate.assets();
    result->m_className = loadableTemplate.className();
    result->m_loadableId = loadableTemplate.id();
    result->m_assetProvider = this;

    for (const auto &f : result->m_initFunctions) {
        f();
    }
    result->m_initFunctions.clear();
    result->m_released = true;
    return result;
}

} // namespace e172
