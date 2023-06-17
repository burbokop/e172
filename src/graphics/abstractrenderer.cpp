// Copyright 2023 Borys Boiko

#include "abstractrenderer.h"

#include <math.h>

#include <src/additional.h>
#include <src/debug.h>

namespace e172 {

Image::Ptr AbstractRenderer::imageProvider(const Image &image)
{
    return image.provider();
}

void AbstractRenderer::drawLine(const Line2d &line, Color color)
{
    const auto& points = line.rectIntersection(resolution());
    drawLine(points.first, points.second, color);
}

void AbstractRenderer::drawLineShifted(const Line2d &line, Color color)
{
    drawLine(line.tanslated(offset()), color);
}

void AbstractRenderer::drawVector(const Vector<double> &position, const Vector<double> &vector, Color color) {
    drawLine(position, position + vector, color);
    drawSquare(position, 2, color);
}

void AbstractRenderer::drawVectorShifted(const Vector<double> &position, const Vector<double> &vector, Color color) {
    drawLineShifted(position, position + vector, color);
    drawSquareShifted(position, 2, color);
}

Vector<double> AbstractRenderer::drawText(const std::string &text,
                                          const Vector<double> &position,
                                          int width,
                                          Color color,
                                          const TextFormat &format)
{
    if (format.fontWidth() == 0)
        return Vector<double>();

    auto lines = Additional::split(text, '\n');
    double offsetY = 0;
    size_t symWidth = width / format.fontWidth();
    auto it = lines.begin();
    while (it != lines.end()) {
        if (it->size() > symWidth) {
            offsetY += drawString(it->substr(0, symWidth),
                                  position + Vector<double>(0, offsetY),
                                  color,
                                  format)
                           .y();
            it = lines.insert(it + 1, it->substr(symWidth, it->size() - symWidth));
        } else {
            offsetY += drawString(*it, position + Vector<double>(0, offsetY), color, format).y();
            ++it;
        }
    }
    return Vector<double>(width, offsetY);
}

Vector<double> AbstractRenderer::offset() const
{
    return resolution() / 2 - cameraPosition();
}

AbstractRenderer::Camera AbstractRenderer::detachCamera() {
    if (!m_cameraLocked) {
        Camera c = Camera::createSharedContainer<Camera>(new Camera::VoidHandle,
                                                         this,
                                                         [this](Camera::DataPtr d) {
                                                             m_cameraLocked = false;
                                                             delete Camera::castHandle<void *>(d);
                                                         });
        c.m_setter = [this](const Vector<double> &vector) { m_position = vector; };
        c.m_getter = [this](){ return m_position; };
        c.m_renderer = this;
        m_cameraLocked = true;
        return c;
    }
    return Camera();
}

Vector<double> AbstractRenderer::cameraPosition() const {
    return m_position;
}

bool AbstractRenderer::isActive() const {
    return m_isValid && !m_locked;
}

void AbstractRenderer::Camera::setPosition(const Vector<double> &position) {
    if (m_setter)
        m_setter(position);
}

Vector<double> AbstractRenderer::Camera::position() const {
    if (m_getter)
        return m_getter();
    return Vector<double>();
}

Color randomColor(Random &random)
{
    return random.next<Color>() % static_cast<Color>(std::pow(2, 24));
}

Color randomColor(Random &&random)
{
    return random.next<Color>() % static_cast<Color>(std::pow(2, 24));
}

Color blendPixels(Color top, Color bottom)
{
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

} // namespace e172
