// Copyright 2023 Borys Boiko

#pragma once

#include "../variant.h"
#include <string>

namespace e172 {

class LoadableTemplate {
public:
    LoadableTemplate() = default;

    LoadableTemplate(const std::string &id, const std::string &className, const VariantMap &assets)
        : m_id(id)
        , m_className(className)
        , m_assets(assets)
        , m_isValid(!className.empty())
    {}

    const std::string &id() const { return m_id; }
    const std::string &className() const { return m_className; }
    const VariantMap &assets() const { return m_assets; }
    bool isValid() const { return m_isValid; }

    friend bool operator ==(const LoadableTemplate& tmpl0, const LoadableTemplate& tmpl1);
    friend bool operator<(const LoadableTemplate &tmpl0, const LoadableTemplate &tmpl1);

private:
    std::string m_id;
    std::string m_className;
    VariantMap m_assets;
    bool m_isValid = false;
};

} // namespace e172
