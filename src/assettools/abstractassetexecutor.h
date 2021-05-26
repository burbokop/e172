#ifndef ABSTRACTASSETEXECUTOR_H
#define ABSTRACTASSETEXECUTOR_H

#include "loadabletemplate.h"

#include <src/variant.h>
#include <src/utility/ptr.h>

namespace e172 {
class AbstractGraphicsProvider;
class AbstractAudioProvider;
class Loadable;
class AssetProvider;

class AbstractAssetExecutor {
    friend AssetProvider;

    AssetProvider* m_provider = nullptr;
    AbstractGraphicsProvider *m_graphicsProvider = nullptr;
    AbstractAudioProvider *m_audioProvider = nullptr;

    std::string executor_path;
public:
    std::string fullPath(const std::string &path);

    LoadableTemplate createTemplate(const e172::VariantMap& object);
    LoadableTemplate loadTemplate(const std::string& templateId);


    AbstractGraphicsProvider *graphicsProvider() const;
    AbstractAudioProvider *audioProvider() const;

    AbstractAssetExecutor();
    virtual ~AbstractAssetExecutor();
    virtual Variant proceed(const Variant &value) = 0;
};


}

#endif // ABSTRACTASSETEXECUTOR_H
