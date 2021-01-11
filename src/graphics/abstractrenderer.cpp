#include "abstractrenderer.h"

#include <math.h>

namespace e172 {


bool AbstractRenderer::isValid() const {
    return m_isValid;
}

bool AbstractRenderer::autoClear() const
{
    return m_autoClear;
}

void AbstractRenderer::setAutoClear(bool autoClear)
{
    m_autoClear = autoClear;
}

Image::ptr AbstractRenderer::imageId(const Image &image) {
    return image.id();
}

AbstractGraphicsProvider *AbstractRenderer::provider() const {
    return m_provider;
}

AbstractRenderer::~AbstractRenderer() {}

Vector AbstractRenderer::offset() const {
    return resolution() / 2 - cameraPosition();
}

AbstractRenderer::Camera AbstractRenderer::detachCamera() {
    if(!m_cameraLocked) {
        Camera c = Camera::newSharedContainer<Camera>(new Camera::void_handle, this, [this](Camera::data_ptr d) {
                m_cameraLocked = false;
                delete Camera::handle_cast<void*>(d);
        });
        c.m_setter = [this](const Vector &vector){ m_position = vector; };
        c.m_getter = [this](){ return m_position; };
        c.m_renderer = this;
        m_cameraLocked = true;
        return c;
    }
    return Camera();
}

Vector AbstractRenderer::cameraPosition() const {
    return m_position;
}

bool AbstractRenderer::isActive() const {
    return m_isValid && !m_locked;
}

AbstractRenderer * AbstractRenderer::Camera::renderer() const {
    return m_renderer;
}

void AbstractRenderer::Camera::setPosition(const Vector &position) {
    if(m_setter)
        m_setter(position);
}

Vector AbstractRenderer::Camera::position() const {
    if(m_getter)
        return m_getter();
    return Vector();
}

Color randomColor(unsigned int seed) {
    return rand_r(&seed) % static_cast<uint32_t>(std::pow(2, 24));
}

Color randomColor() {
    return rand() % static_cast<uint32_t>(std::pow(2, 24));
}

Color blendPixels(Color top, Color bottom) {
    const auto bA = uint8_t((top >>  0) & 0x000000ff);
    const auto gA = uint8_t((top >>  8) & 0x000000ff);
    const auto rA = uint8_t((top >> 16) & 0x000000ff);
    const auto aA = uint8_t((top >> 24) & 0x000000ff);

    const auto bB = uint8_t((bottom >>  0) & 0x000000ff);
    const auto gB = uint8_t((bottom >>  8) & 0x000000ff);
    const auto rB = uint8_t((bottom >> 16) & 0x000000ff);
    const auto aB = uint8_t((bottom >> 24) & 0x000000ff);

    const uint32_t bOut = uint32_t((bA * aA / 255) + (bB * aB * (255 - aA) / (255 * 255))) << 0;
    const uint32_t gOut = uint32_t((gA * aA / 255) + (gB * aB * (255 - aA) / (255 * 255))) << 8;
    const uint32_t rOut = uint32_t((rA * aA / 255) + (rB * aB * (255 - aA) / (255 * 255))) << 16;
    const uint32_t aOut = uint32_t(aA + (aB * (255 - aA) / 255)) << 24;

    return aOut | rOut | gOut | bOut;
}

}
