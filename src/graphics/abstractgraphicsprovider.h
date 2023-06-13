#ifndef ABSTRACTRENDERENGINE_H
#define ABSTRACTRENDERENGINE_H

#include "abstractrenderer.h"
#include "image.h"
#include <string>


namespace e172 {


class AbstractGraphicsProvider {
    std::vector<std::string> m_args;
protected:
    Image imageFromData(Image::data_ptr data, std::size_t w, std::size_t h) const;
    void installParentToRenderer(AbstractRenderer *renderer);
public:
    using ImageInitFunction = std::function<void(uint32_t *)>;
    using ImageInitFunctionExt = std::function<void(size_t, size_t, uint32_t *)>;

    AbstractGraphicsProvider(const std::vector<std::string> &args)
        : m_args(args)
    {}

    virtual AbstractRenderer *renderer() const = 0;
    virtual bool isValid() const = 0;
    virtual Image loadImage(const std::string &path) const = 0;
    virtual Image createImage(int width, int height) const = 0;
    virtual Image createImage(int width, int height, const ImageInitFunction &imageInitFunction) const = 0;
    virtual Image createImage(int width, int height, const ImageInitFunctionExt &imageInitFunction) const = 0;
    virtual void loadFont(const std::string &name, const std::string &path) = 0;
    virtual bool fontLoaded(const std::string &name) const = 0;

    virtual ~AbstractGraphicsProvider() = default;
    std::vector<std::string> args() const;

protected:
    virtual void destructImage(Image::data_ptr ptr) const = 0;
    virtual Image::ptr imageBitMap(Image::data_ptr ptr) const = 0;
    virtual bool saveImage(Image::data_ptr, const std::string&) const = 0;
    virtual Image::data_ptr imageFragment(
        Image::data_ptr ptr, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h) const
        = 0;

    virtual Image::data_ptr blitImages(Image::data_ptr ptr0,
                                       Image::data_ptr ptr1,
                                       int x,
                                       int y,
                                       std::size_t &w,
                                       std::size_t &h) const
        = 0;

    virtual Image::data_ptr transformImage(Image::data_ptr, uint64_t) const = 0;
};

}

#endif // ABSTRACTRENDERENGINE_H
