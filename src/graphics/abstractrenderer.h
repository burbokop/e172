#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <inttypes.h>

#include "image.h"
#include "shapeformat.h"
#include "../math/vector.h"
#include "textformat.h"
#include "../variant.h"
#include "../math/line2d.h"

namespace e172 {

typedef uint32_t Color;
typedef uint8_t ColorComponent;

/**
 * @brief argb
 * @param alpha
 * @param r
 * @param g
 * @param b
 * @return color
 * @note all arguments from 0 to 255
 */
inline Color argb(ColorComponent alpha, ColorComponent r, ColorComponent g, ColorComponent b) {
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
inline Color rgb(ColorComponent r, ColorComponent g, ColorComponent b) {
    return argb(0xff, r, g, b);
}

inline ColorComponent alpha(Color color) { return color >> 24; }

inline ColorComponent red(Color color) { return color >> 16; }

inline ColorComponent green(Color color) { return color >> 8; }

inline ColorComponent blue(Color color) { return color >> 0; }

Color randomColor();
Color randomColor(unsigned int seed);

Color blendPixels(Color top, Color bottom);

class AbstractGraphicsProvider;
class AbstractRenderer {
    friend AbstractGraphicsProvider;
    bool m_isValid = false;
    bool m_locked = true;
    AbstractGraphicsProvider *m_provider = nullptr;
    Vector m_position;
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
        typedef std::function<void(const Vector &)> setter_t;
        typedef std::function<Vector()> getter_t;

        setter_t m_setter;
        getter_t m_getter;
    public:
        AbstractRenderer *renderer() const;
        void setPosition(const Vector &position);
        Vector position() const;
    };
protected:
    template<typename T>
    static T imageData(const Image &image) { return image.casted_handle<T>()->c; }
    static Image::ptr imageProvider(const Image &image);
    AbstractGraphicsProvider *provider() const;
public:
    virtual ~AbstractRenderer();

    virtual size_t presentEffectCount() const = 0;
    virtual std::string presentEffectName(size_t index) const = 0;
    virtual void drawEffect(size_t index, const e172::VariantVector &args = e172::VariantVector()) = 0;

    virtual void setDepth(int64_t depth) = 0;
    virtual void fill(uint32_t color) = 0;
    virtual void drawPixel(const Vector &point, uint32_t color) = 0;
    virtual void drawLine(const Vector &point0, const Vector &point1, uint32_t color) = 0;
    virtual void drawRect(const Vector &point0, const Vector &point1, uint32_t color, const ShapeFormat& format = ShapeFormat(false)) = 0;
    virtual void drawSquare(const Vector &point, int radius, uint32_t color) = 0;
    virtual void drawCircle(const Vector &point, int radius, uint32_t color) = 0;
    virtual void drawDiagonalGrid(const Vector &point0, const Vector &point1, int interval, uint32_t color) = 0;
    virtual void drawImage(const Image &image, const Vector &position, double angle, double zoom) = 0;
    virtual Vector drawString(const std::string &string, const Vector &position, uint32_t color, const TextFormat &format = TextFormat()) = 0;

    virtual void modify_bitmap(const std::function<void(Color* bitmap)>& modifier) = 0;

    inline void drawPixelShifted(const Vector &point, uint32_t color) { drawPixel(point + offset(), color); }
    inline void drawLineShifted(const Vector &point0, const Vector &point1, uint32_t color) { drawLine(point0 + offset(), point1 + offset(), color); }
    inline void drawRectShifted(const Vector &point0, const Vector &point1, uint32_t color) { drawRect(point0 + offset(), point1 + offset(), color); }
    inline void drawSquareShifted(const Vector &point, int radius, uint32_t color) { drawSquare(point + offset(), radius, color); }
    inline void drawCircleShifted(const Vector &point, int radius, uint32_t color) { drawCircle(point + offset(), radius, color); }
    inline void drawDiagonalGridShifted(const Vector &point0, const Vector &point1, int interval, uint32_t color) { drawDiagonalGrid(point0 + offset(), point1 + offset(), interval, color); }
    inline void drawImageShifted(const Image &image, const Vector &position, double angle, double zoom) { drawImage(image, position + offset(), angle, zoom); }
    inline Vector drawStringShifted(const std::string &string, const Vector &position, uint32_t color, const TextFormat &format = TextFormat()) { return drawString(string, position + offset(), color, format); }

    void drawLine(const Line2d &line, uint32_t color);
    void drawLineShifted(const Line2d &line, uint32_t color);

    void drawVector(const e172::Vector& position, const e172::Vector& vector, e172::Color color);
    void drawVectorShifted(const e172::Vector& position, const e172::Vector& vector, e172::Color color);

    Vector drawText(const std::string &text, const Vector &position, int width, uint32_t color, const TextFormat &format = TextFormat());

    virtual void applyLensEffect(const Vector &point0, const Vector &point1, double coefficient) = 0;
    virtual void applySmooth(const Vector &point0, const Vector &point1, double coefficient) = 0;
    virtual void enableEffect(uint64_t effect) = 0;
    virtual void disableEffect(uint64_t effect) = 0;

    virtual void setFullscreen(bool value) = 0;
    virtual void setResolution(Vector value) = 0;
    virtual Vector resolution() const = 0;
    virtual Vector screenSize() const = 0;

    Vector offset() const;
    Camera detachCamera();
    Vector cameraPosition() const;
    bool isActive() const;
    bool isValid() const;
    bool autoClear() const;
    void setAutoClear(bool autoClear);
};

}

#endif // ABSTRACTRENDERER_H
