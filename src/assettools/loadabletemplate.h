#ifndef LOADABLETEMPLATE_H
#define LOADABLETEMPLATE_H

#include <src/variant.h>

namespace e172 {


class LoadableTemplate {
    std::string m_id;
    std::string m_className;
    VariantMap m_assets;
    bool m_isValid = false;
public:
    LoadableTemplate() {}
    LoadableTemplate(const std::string &id, const std::string &className, const VariantMap &assets);
    std::string id() const;
    std::string className() const;
    VariantMap assets() const;
    bool isValid() const;

    friend bool operator ==(const LoadableTemplate& tmpl0, const LoadableTemplate& tmpl1);
    friend bool operator <(const LoadableTemplate& tmpl0, const LoadableTemplate& tmpl1);
};

}
#endif // LOADABLETEMPLATE_H
