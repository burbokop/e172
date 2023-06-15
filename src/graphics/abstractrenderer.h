#pragma once

#include "image.h"
#include "shapeformat.h"
#include "../math/vector.h"
#include "textformat.h"
#include "../variant.h"
#include "../math/line2d.h"

namespace e172 {

using Color = std::uint32_t;
using ColorComponent = std::uint8_t;

/**
 * @brief argb
 * @param alpha
 * @param r
 * @param g
 * @param b
 * @return color
 * @note all arguments from 0 to 255
 */
constexpr Color argb(ColorComponent alpha, ColorComponent r, ColorComponent g, ColorComponent b)
{
    return (static_cast<Color>(alpha) << 24)
            | (static_cast<Color>(r) << 16)
            | (static_cast<Color>(g) << 8)
            | static_cast<Color>(b);
}

/**
 * @brief rgb
 * @param r
 * @param g
 * @param b
 * @return
 * @note all arguments from 0 to 255
 */
constexpr Color rgb(ColorComponent r, ColorComponent g, ColorComponent b)
{
    return argb(0xff, r, g, b);
}

constexpr ColorComponent alpha(Color color)
{
    return color >> 24;
}

constexpr ColorComponent red(Color color)
{
    return color >> 16;
}

constexpr ColorComponent green(Color color)
{
    return color >> 8;
}

constexpr ColorComponent blue(Color color)
{
    return color >> 0;
}

Color randomColor(Random &random);
Color randomColor(Random &&random);

Color blendPixels(Color top, Color bottom);

class AbstractGraphicsProvider;
class AbstractRenderer {
    friend AbstractGraphicsProvider;
    bool m_isValid = false;
    bool m_locked = true;
    AbstractGraphicsProvider *m_provider = nullptr;
    Vector<double> m_position;
    bool m_cameraLocked = false;
    bool m_autoClear = true;

    /**
     * Only GameApplication class cann call update function
     */
    friend class GameApplication;
protected:
    virtual bool update() = 0;
public:
    class Camera : public SharedContainer {
        friend AbstractRenderer;
        AbstractRenderer *m_renderer = nullptr;
        typedef std::function<void(const Vector<double> &)> setter_t;
        typedef std::function<Vector<double>()> getter_t;

        setter_t m_setter;
        getter_t m_getter;
    public:
        AbstractRenderer *renderer() const;
        void setPosition(const Vector<double> &position);
        Vector<double> position() const;
    };
protected:
    template<typename T>
    static T imageData(const Image &image) { return image.casted_handle<T>()->c; }
    static Image::ptr imageProvider(const Image &image);
    AbstractGraphicsProvider *provider() const;

public:
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
                                     uint32_t color,
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
    Vector<double> cameraPosition() const;
    bool isActive() const;
    bool isValid() const;
    bool autoClear() const;
    void setAutoClear(bool autoClear);

    // Virtual methods
public:
    virtual size_t presentEffectCount() const = 0;
    virtual std::string presentEffectName(size_t index) const = 0;
    virtual void drawEffect(size_t index, const e172::VariantVector &args = e172::VariantVector()) = 0;

    virtual void setDepth(int64_t depth) = 0;
    virtual void fill(uint32_t color) = 0;
    virtual void drawPixel(const Vector<double> &point, uint32_t color) = 0;
    virtual void drawLine(const Vector<double> &point0, const Vector<double> &point1, Color color)
        = 0;

    virtual void drawRect(const Vector<double> &point0,
                          const Vector<double> &point1,
                          uint32_t color,
                          const ShapeFormat &format = ShapeFormat(false))
        = 0;

    virtual void drawSquare(const Vector<double> &point, int radius, Color color) = 0;
    virtual void drawCircle(const Vector<double> &point, int radius, Color color) = 0;
    virtual void drawDiagonalGrid(const Vector<double> &point0,
                                  const Vector<double> &point1,
                                  int interval,
                                  Color color)
        = 0;

    virtual void drawImage(const Image &image,
                           const Vector<double> &position,
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
    virtual void setResolution(Vector<double> value) = 0;
    virtual Vector<double> resolution() const = 0;
    virtual Vector<double> screenSize() const = 0;

    virtual ~AbstractRenderer();
};

} // namespace e172
