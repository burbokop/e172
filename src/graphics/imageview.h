// Copyright 2023 Borys Boiko

#pragma once

#include "../entity.h"
#include "../math/vector.h"
#include "image.h"
#include <utility>

namespace e172 {

class ImageView : public Entity {
public:
    ImageView(FactoryMeta &&meta, const Image &image = Image())
        : Entity(std::move(meta))
        , m_image(image)
    {}

    Image image() const { return m_image; }
    void setImage(const Image &image) { m_image = image; }
    Vector<double> position() const { return m_position; }
    void setPosition(const Vector<double> &position) { m_position = position; }
    double rotation() const { return m_rotation; }
    void setRotation(double rotation) { m_rotation = rotation; }
    double scale() const { return m_scale; }
    void setScale(double scale) { m_scale = scale; }

    // Entity interface
public:
    void proceed(Context *, EventHandler *) override;
    void render(e172::Context *context, AbstractRenderer *renderer) override;

private:
    Image m_image;
    Vector<double> m_position;
    double m_rotation = 0;
    double m_scale = 1;
};

} // namespace e172
