#include "abstractrenderer.h"
#include "imageview.h"

#include <src/math/math.h>

e172::ImageView::ImageView(FactoryMeta &&meta, const Image &image)
    : Entity(std::move(meta))
    , m_image(image)
{}

void e172::ImageView::proceed(e172::Context *, e172::EventHandler *) {}

void e172::ImageView::render(e172::AbstractRenderer *renderer) {
    if(m_image.isValid() && m_scale != e172::Math::null)
        renderer->drawImageShifted(m_image, m_position, m_rotation, m_scale);
}

e172::Image e172::ImageView::image() const {
    return m_image;
}

void e172::ImageView::setImage(const Image &image) {
    m_image = image;
}


namespace e172 {

double ImageView::scale() const
{
    return m_scale;
}

void ImageView::setScale(double scale)
{
    m_scale = scale;
}



double ImageView::rotation() const
{
    return m_rotation;
}

void ImageView::setRotation(double rotation)
{
    m_rotation = rotation;
}


Vector<double> ImageView::position() const
{
    return m_position;
}

void ImageView::setPosition(const Vector<double> &position)
{
    m_position = position;
}

}
