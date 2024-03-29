// Copyright 2023 Borys Boiko

#pragma once

#include "../math/line2d.h"
#include "../math/vector.h"
#include "../variant.h"
#include "color.h"
#include "image.h"
#include "shapeformat.h"
#include "textformat.h"
#include <string>

namespace e172 {

class GameApplication;
class AbstractGraphicsProvider;

class AbstractRenderer {
    friend AbstractGraphicsProvider;

    /**
     * Only GameApplication class cann call update function
     */
    friend GameApplication;

public:
    class Camera : public SharedContainer {
        friend AbstractRenderer;
        using Setter = std::function<void(const Vector<double> &)>;
        using Getter = std::function<Vector<double>()>;

    public:
        AbstractRenderer *renderer() const { return m_renderer; }
        void setPosition(const Vector<double> &position);
        Vector<double> position() const;

    private:
        AbstractRenderer *m_renderer = nullptr;
        Setter m_setter;
        Getter m_getter;
    };

    void drawPixelShifted(const Vector<double> &point, Color color)
    {
        drawPixel(point + offset(), color);
    }

    void drawLineShifted(const Vector<double> &point0, const Vector<double> &point1, Color color)
    {
        drawLine(point0 + offset(), point1 + offset(), color);
    }

    void drawRectShifted(const Vector<double> &point0, const Vector<double> &point1, Color color)
    {
        drawRect(point0 + offset(), point1 + offset(), color);
    }

    void drawSquareShifted(const Vector<double> &point, int radius, Color color)
    {
        drawSquare(point + offset(), radius, color);
    }

    void drawCircleShifted(const Vector<double> &point, int radius, Color color)
    {
        drawCircle(point + offset(), radius, color);
    }

    void drawDiagonalGridShifted(const Vector<double> &point0,
                                 const Vector<double> &point1,
                                 int interval,
                                 Color color)
    {
        drawDiagonalGrid(point0 + offset(), point1 + offset(), interval, color);
    }

    void drawImageShifted(const Image &image,
                          const Vector<double> &position,
                          double angle,
                          double zoom)
    {
        drawImage(image, position + offset(), angle, zoom);
    }

    Vector<double> drawStringShifted(const std::string &string,
                                     const Vector<double> &position,
                                     Color color,
                                     const TextFormat &format = TextFormat())
    {
        return drawString(string, position + offset(), color, format);
    }

    void drawLine(const Line2d &line, Color color);
    void drawLineShifted(const Line2d &line, Color color);

    void drawVector(const Vector<double> &position, const Vector<double> &vector, Color color);
    void drawVectorShifted(const Vector<double> &position,
                           const Vector<double> &vector,
                           Color color);

    Vector<double> drawText(const std::string &text,
                            const Vector<double> &position,
                            int width,
                            Color color,
                            const TextFormat &format = TextFormat());

    Vector<double> offset() const;
    Camera detachCamera();
    Vector<double> cameraPosition() const { return m_position; }
    bool isActive() const;
    bool isValid() const { return m_isValid; }
    bool autoClear() const { return m_autoClear; }
    void setAutoClear(bool autoClear) { m_autoClear = autoClear; }
    const AbstractGraphicsProvider *provider() const { return m_provider; }

    // Virtual methods
public:
    virtual std::size_t presentEffectCount() const = 0;
    virtual std::string presentEffectName(std::size_t index) const = 0;
    virtual void drawEffect(size_t index, const e172::VariantVector &args = e172::VariantVector()) = 0;

    virtual void setDepth(int64_t depth) = 0;
    virtual void fill(Color color) = 0;
    virtual void drawPixel(const Vector<double> &point, Color color) = 0;
    virtual void drawLine(const Vector<double> &point0, const Vector<double> &point1, Color color)
        = 0;

    virtual void drawRect(const Vector<double> &point0,
                          const Vector<double> &point1,
                          Color color,
                          const ShapeFormat &format = ShapeFormat(false))
        = 0;

    virtual void drawSquare(const Vector<double> &center, double radius, Color color) = 0;
    virtual void drawCircle(const Vector<double> &center, double radius, Color color) = 0;
    virtual void drawDiagonalGrid(const Vector<double> &point0,
                                  const Vector<double> &point1,
                                  double interval,
                                  Color color)
        = 0;

    virtual void drawImage(const Image &image,
                           const Vector<double> &center,
                           double angle,
                           double zoom)
        = 0;

    virtual Vector<double> drawString(const std::string &string,
                                      const Vector<double> &position,
                                      Color color,
                                      const TextFormat &format = TextFormat())
        = 0;

    virtual void modifyBitmap(const std::function<void(Color *bitmap)> &modifier) = 0;

    virtual void applyLensEffect(const Vector<double> &point0,
                                 const Vector<double> &point1,
                                 double coefficient)
        = 0;

    virtual void applySmooth(const Vector<double> &point0,
                             const Vector<double> &point1,
                             double coefficient)
        = 0;

    virtual void enableEffect(std::uint64_t effect) = 0;
    virtual void disableEffect(std::uint64_t effect) = 0;

    virtual void setFullscreen(bool value) = 0;
    virtual void setResolution(const Vector<std::uint32_t> &value) = 0;
    virtual Vector<std::uint32_t> resolution() const = 0;

    virtual ~AbstractRenderer() = default;

protected:
    template<typename T>
    static T imageData(const Image &image)
    {
        return image.handleAs<T>()->c;
    }

    static Image::Ptr imageProvider(const Image &image);

    virtual bool update() = 0;

private:
    bool m_isValid = false;
    bool m_locked = true;
    const AbstractGraphicsProvider *m_provider = nullptr;
    Vector<double> m_position;
    bool m_cameraLocked = false;
    bool m_autoClear = true;
};

} // namespace e172
