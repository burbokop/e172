#include "abstractrenderer.h"

#include <math.h>

#include <src/additional.h>
#include <src/debug.h>

namespace e172 {


bool AbstractRenderer::isValid() const {
    return m_isValid;
}

bool AbstractRenderer::autoClear() const {
    return m_autoClear;
}

void AbstractRenderer::setAutoClear(bool autoClear) {
    m_autoClear = autoClear;
}

Image::ptr AbstractRenderer::imageProvider(const Image &image) {
    return image.provider();
}

AbstractGraphicsProvider *AbstractRenderer::provider() const {
    return m_provider;
}

AbstractRenderer::~AbstractRenderer() {}

void AbstractRenderer::drawLine(const Line2d &line, uint32_t color) {
    const auto& points = line.rectIntersection(resolution());
    drawLine(points.first, points.second, color);
}

void AbstractRenderer::drawLineShifted(const Line2d &line, uint32_t color) {
    drawLine(line.tanslated(offset()), color);
}

void AbstractRenderer::drawVector(const Vector &position, const Vector &vector, Color color) {
    drawLine(position, position + vector, color);
    drawSquare(position, 2, color);
}

void AbstractRenderer::drawVectorShifted(const Vector &position, const Vector &vector, Color color) {
    drawLineShifted(position, position + vector, color);
    drawSquareShifted(position, 2, color);
}

Vector AbstractRenderer::drawText(const std::string &text, const Vector &position, int width, uint32_t color, const TextFormat &format) {
    if(format.fontWidth() == 0)
        return Vector();

    auto lines = Additional::split(text, '\n');
    double offsetY = 0;
    size_t symWidth = width / format.fontWidth();
    auto it = lines.begin();
    while (it != lines.end()) {
        if(it->size() > symWidth) {
            offsetY += drawString(it->substr(0, symWidth), position + e172::Vector(0, offsetY), color, format).y();
            it = lines.insert(it + 1, it->substr(symWidth, it->size() - symWidth));
        } else {
            offsetY += drawString(*it, position + e172::Vector(0, offsetY), color, format).y();
            ++it;
        }
    }
    return e172::Vector(width, offsetY);
}

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
