// Copyright 2023 Borys Boiko

#include "assetprovider.h"

#include "abstractassetexecutor.h"
#include <src/additional.h>
#include <src/context.h>
#include <src/debug.h>
#include <src/graphics/abstractgraphicsprovider.h>

namespace e172 {

void AssetProvider::searchInFolder(const std::filesystem::path &dir)
{
    for (auto const &entry : std::filesystem::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            processFile(entry.path().string());
        }
    }
}

Either<AssetProvider::Error, Loadable *> AssetProvider::createLoadable(const std::string &templateId)
{
    const auto it = m_templates.find(templateId);
    if (it == m_templates.end()) {
        return Left(TemplateNotFound{.id = templateId}.toErr());
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

void AssetProvider::processFile(const std::filesystem::path &file)
{
    assert(std::filesystem::is_regular_file(file));
    const auto path = file.parent_path().string();

    // TODO(burbokop): rewrite with std::filesystem
    std::string sufix = Additional::fileSufix(file.string());
    if (sufix == ".json") {
        const auto root = Variant::fromJson(Additional::readFile(file.string())).toMap();
        if (root.size() == 0) {
            Debug::warning("Empty json object detected or parsing error. file:", file);
            return;
        }
        const auto t = parseTemplate(root, path);
        if (t.isValid()) {
            addTemplate(t);
        }
    }
}

LoadableTemplate AssetProvider::parseTemplate(const VariantMap &root, const std::string &path)
{
    const auto id = Additional::value(root, "id");
    const auto className = Additional::value(root, "class");

    if (id.isNull()) {
        Debug::warning("Template id missing. object:", root);
        return LoadableTemplate();
    }

    if (className.isNull()) {
        Debug::warning("Template class name missing. object:", root);
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

Either<e172::AssetProvider::Error, Loadable *> AssetProvider::createLoadable(
    const LoadableTemplate &loadableTemplate)
{
    auto result = m_factory.create(loadableTemplate.className());
    if (!result) {
        return Left(TypeNotRegistered{.typeName = loadableTemplate.className(),
                                      .templateId = loadableTemplate.id()}
                        .toErr());
    }

    result->m_assets = loadableTemplate.assets();
    result->m_loadableClassName = loadableTemplate.className();
    result->m_templateId = loadableTemplate.id();
    result->m_assetProvider = this;

    for (const auto &f : result->m_initFunctions) {
        f();
    }
    result->m_initFunctions.clear();
    result->m_released = true;
    return Right(result);
}

} // namespace e172
