// Copyright 2023 Borys Boiko

#include "imageview.h"

#include "../math/math.h"
#include "abstractrenderer.h"

void e172::ImageView::proceed(e172::Context *, e172::EventHandler *) {}

void e172::ImageView::render(e172::Context *context, e172::AbstractRenderer *renderer)
{
    if (m_image.isValid() && m_scale != e172::Math::null) {
        renderer->drawImageShifted(m_image, m_position, m_rotation, m_scale);
    }
}
