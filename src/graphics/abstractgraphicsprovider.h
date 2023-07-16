// Copyright 2023 Borys Boiko

#pragma once

#include "abstractrenderer.h"
#include "image.h"
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

namespace e172 {

class AbstractGraphicsProvider {
public:
    using ImageInitFunction = std::function<void(Color *)>;
    using ImageInitFunctionExt = std::function<void(std::size_t, std::size_t, Color *)>;

    AbstractGraphicsProvider() = default;

    virtual std::shared_ptr<AbstractRenderer>
    createRenderer(const std::string &title, const Vector<std::uint32_t> &resolution) const = 0;

    virtual Image loadImage(const std::string &path) const = 0;
    virtual Image createImage(std::size_t width, std::size_t height) const = 0;
    virtual Image createImage(std::size_t width,
                              std::size_t height,
                              const ImageInitFunction &imageInitFunction) const
        = 0;
    virtual Image createImage(std::size_t width,
                              std::size_t height,
                              const ImageInitFunctionExt &imageInitFunction) const
        = 0;
    virtual void loadFont(const std::string &name, const std::filesystem::path &path) = 0;
    virtual bool fontLoaded(const std::string &name) const = 0;
    virtual Vector<std::uint32_t> screenSize() const = 0;

    virtual ~AbstractGraphicsProvider() = default;

protected:
    Image imageFromData(Image::DataPtr data, std::size_t w, std::size_t h) const;
    void installParentToRenderer(AbstractRenderer &renderer) const;

    virtual void destructImage(Image::DataPtr ptr) const = 0;
    virtual Image::Ptr imageBitMap(Image::DataPtr ptr) const = 0;
    virtual bool saveImage(Image::DataPtr, const std::string &) const = 0;
    virtual Image::DataPtr imageFragment(
        Image::DataPtr ptr, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h) const
        = 0;

    virtual Image::DataPtr blitImages(Image::DataPtr ptr0,
                                      Image::DataPtr ptr1,
                                      std::ptrdiff_t x,
                                      std::ptrdiff_t y,
                                      std::size_t &w,
                                      std::size_t &h) const
        = 0;

    virtual Image::DataPtr transformImage(Image::DataPtr, std::uint64_t) const = 0;
};

} // namespace e172
