#include "abstractassetexecutor.h"

#include <src/additional.h>
#include "assetprovider.h"

namespace e172 {

AbstractGraphicsProvider *AbstractAssetExecutor::graphicsProvider() const
{
    return m_graphicsProvider;
}

AbstractAudioProvider *AbstractAssetExecutor::audioProvider() const
{
    return m_audioProvider;
}

std::string AbstractAssetExecutor::fullPath(const std::string &path) {
    return Additional::concatenatePaths(executor_path, path);
}

Loadable *AbstractAssetExecutor::createLoadable(const VariantMap &object) {
    if(m_provider) {
        const auto pair = m_provider->createTemplate(object, executor_path);
        if(!pair.first.empty()) {
            return m_provider->createLoadable(pair.first, pair.second);
        }
    }
    return nullptr;
}

AbstractAssetExecutor::AbstractAssetExecutor() {}

AbstractAssetExecutor::~AbstractAssetExecutor() {}


}
