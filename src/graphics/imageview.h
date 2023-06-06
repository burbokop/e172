#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "../entity.h"

#include "image.h"

#include <src/math/vector.h>

namespace e172 {

class ImageView : public Entity {
    Image m_image;
    Vector m_position;
    double m_rotation = 0;
    double m_scale = 1;
public:
    ImageView(const Image &image = Image());

    // Entity interface
public:
    virtual void proceed(Context *, EventHandler *) override;
    virtual void render(AbstractRenderer *renderer) override;
    Image image() const;
    void setImage(const Image &image);
    Vector position() const;
    void setPosition(const Vector &position);
    double rotation() const;
    void setRotation(double rotation);
    double scale() const;
    void setScale(double scale);
};
}
#endif // IMAGEVIEW_H
