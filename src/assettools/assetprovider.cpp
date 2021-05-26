#include "assetprovider.h"

#include <src/debug.h>
#include <src/graphics/abstractgraphicsprovider.h>
#include <src/additional.h>
#include <src/context.h>
#include "abstractassetexecutor.h"

namespace e172 {


AssetProvider::AssetProvider() {}


void AssetProvider::searchInFolder(std::string path) {
    if(path[path.length() - 1] == '/') path.pop_back();
    std::vector<std::string> items = Additional::directoryContent(path);
    for(unsigned long long i = 0, L = items.size(); i < L; i++) {
        std::string item = items[i];
        std::string file = path + '/' + item;
        if(Additional::isDirectory(file)) {
            searchInFolder(file);
        } else {
            processFile(file, path);
        }
    }
}

Loadable *AssetProvider::createLoadable(const std::string &templateId) {
    const auto it = templates.find(templateId);
    if(it == templates.end()) {
        Debug::warning("AssetProvider::createLoadable: Loadable template not found for id:", templateId);
        return nullptr;
    }
    return createLoadable(it->first, it->second);
}

std::vector<std::string> AssetProvider::loadableNames() {
    std::vector<std::string> result;
    for (auto it = templates.begin(); it != templates.end(); it++) {
        result.push_back(it->first);
    }
    return result;
}

void AssetProvider::addTemplate(const std::string &templateName, const std::string &className, const VariantMap &assets) {
    templates[templateName] = { className, assets };
}

void AssetProvider::installExecutor(const std::string &id, const std::shared_ptr<AbstractAssetExecutor> &executor) {
    executors[id] = executor;
}

void AssetProvider::processFile(std::string file, std::string path) {
    std::string sufix = Additional::fileSufix(file);
    if(sufix == ".json") {
        const auto root = Variant::fromJson(Additional::readFile(file)).toMap();
        if(root.size() == 0) {
            Debug::warning("Empty json object detected or parsing error. file:", file);
            return;
        }
        const auto t = createTemplate(root, path);
        if(!t.first.empty()) {
            templates[t.first] = t.second;
        }
    }
}

std::pair<std::string, AssetProvider::LoadableTemplate> AssetProvider::createTemplate(const VariantMap &root, const std::string &path) {
    const auto id = Additional::value(root, "id");
    const auto className = Additional::value(root, "class");
    if(id.isNull() || className.isNull()) {
        if(id.isNull()) {
            Debug::warning("Template id missing. object:", root);
        } else {
            Debug::warning("Template class name missing. object:", root);
        }
        return { "", LoadableTemplate{} };
    }

    LoadableTemplate result;
    result.className = className.toString();
    for(auto item = root.begin(); item != root.end(); ++item) {
        const auto& assetId = item->first;
        if(assetId != "class" && assetId != "id") {
            if(item->second.isNull()) {
                Debug::warning("Asset is null. Id:", assetId, "object:", root);
            } else {
                auto it = executors.find(assetId);
                if(it != executors.end() && it->second) {
                    it->second->executor_path = m_context->absolutePath(path);
                    it->second->m_provider = this;
                    it->second->m_graphicsProvider = m_graphicsProvider;
                    it->second->m_audioProvider = m_audioProvider;
                    result.assets[assetId] = it->second->proceed(item->second);
                } else {
                    result.assets[assetId] = item->second;
                }
            }
        }
    }
    return { id.toString(), result };
}

Loadable *AssetProvider::createLoadable(const std::string &templateId, const AssetProvider::LoadableTemplate &loadableTemplate) {
    auto result = m_factory.create(loadableTemplate.className);
    if(!result) {
        Debug::warning("AssetProvider::createLoadable: Type not registered:", loadableTemplate.className, "( template id: ", templateId, ")");
        return nullptr;
    }

    result->m_assets = loadableTemplate.assets;
    result->m_className = loadableTemplate.className;
    result->m_loadableId = templateId;
    result->m_assetProvider = this;

    for(const auto& f : result->initialize_functions) {
        f();
    }
    result->initialize_functions.clear();
    result->released = true;
    return result;
}

}
