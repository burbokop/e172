// Copyright 2023 Borys Boiko

#include "loadabletemplate.h"

namespace e172 {

bool operator==(const LoadableTemplate &tmpl0, const LoadableTemplate &tmpl1)
{
    return tmpl0.m_id == tmpl1.m_id && tmpl0.m_assets == tmpl1.m_assets
           && tmpl0.m_className == tmpl1.m_className && tmpl0.m_isValid == tmpl1.m_isValid;
}

bool operator<(const LoadableTemplate &tmpl0, const LoadableTemplate &tmpl1)
{
    return tmpl0.m_id < tmpl1.m_id && tmpl0.m_assets < tmpl1.m_assets
           && tmpl0.m_className < tmpl1.m_className && tmpl0.m_isValid < tmpl1.m_isValid;
}

} // namespace e172
