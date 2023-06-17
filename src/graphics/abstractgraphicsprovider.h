// Copyright 2023 Borys Boiko

#pragma once

#include "abstractrenderer.h"
#include "image.h"
#include <string>
#include <vector>

namespace e172 {

class AbstractGraphicsProvider {
public:
    using ImageInitFunction = std::function<void(std::uint32_t *)>;
    using ImageInitFunctionExt = std::function<void(std::size_t, std::size_t, std::uint32_t *)>;

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
    std::vector<std::string> args() const { return m_args; }

protected:
    Image imageFromData(Image::DataPtr data, std::size_t w, std::size_t h) const;
    void installParentToRenderer(AbstractRenderer *renderer);

    virtual void destructImage(Image::DataPtr ptr) const = 0;
    virtual Image::Ptr imageBitMap(Image::DataPtr ptr) const = 0;
    virtual bool saveImage(Image::DataPtr, const std::string &) const = 0;
    virtual Image::DataPtr imageFragment(
        Image::DataPtr ptr, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h) const
        = 0;

    virtual Image::DataPtr blitImages(
        Image::DataPtr ptr0, Image::DataPtr ptr1, int x, int y, std::size_t &w, std::size_t &h) const
        = 0;

    virtual Image::DataPtr transformImage(Image::DataPtr, uint64_t) const = 0;

private:
    std::vector<std::string> m_args;
};

} // namespace e172
