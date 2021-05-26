#include "loadabletemplate.h"


namespace e172 {
std::string LoadableTemplate::className() const {
    return m_className;
}

VariantMap LoadableTemplate::assets() const {
    return m_assets;
}

bool LoadableTemplate::isValid() const {
    return m_isValid;
}

bool operator ==(const LoadableTemplate &tmpl0, const LoadableTemplate &tmpl1) {
    return tmpl0.m_id == tmpl1.m_id && tmpl0.m_assets == tmpl1.m_assets && tmpl0.m_className == tmpl1.m_className && tmpl0.m_isValid == tmpl1.m_isValid;
}

bool operator <(const LoadableTemplate &tmpl0, const LoadableTemplate &tmpl1) {
    return tmpl0.m_id < tmpl1.m_id && tmpl0.m_assets < tmpl1.m_assets && tmpl0.m_className < tmpl1.m_className && tmpl0.m_isValid < tmpl1.m_isValid;
}

LoadableTemplate::LoadableTemplate(const std::string &id, const std::string &className, const VariantMap &assets) {
    m_id = id;
    m_className = className;
    m_assets = assets;
    m_isValid = !className.empty();
}

std::string LoadableTemplate::id() const {
    return m_id;
}

}
