// Copyright 2023 Borys Boiko

#include "abstractassetexecutor.h"

#include <src/additional.h>
#include "assetprovider.h"

namespace e172 {

std::string AbstractAssetExecutor::fullPath(const std::string &path) {
    return Additional::concatenatePaths(m_executorPath, path);
}

e172::LoadableTemplate e172::AbstractAssetExecutor::createTemplate(const e172::VariantMap &object)
{
    if (m_provider) {
        return m_provider->parseTemplate(object, m_executorPath);
    }
    return LoadableTemplate();
}

LoadableTemplate AbstractAssetExecutor::loadTemplate(const std::string &templateId)
{
    if (m_provider) {
        const auto it = m_provider->m_templates.find(templateId);
        if (it != m_provider->m_templates.end()) {
            return it->second;
        }
    }
    return LoadableTemplate();
}

} // namespace e172
