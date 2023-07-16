// Copyright 2023 Borys Boiko

#include "abstractgraphicsprovider.h"

namespace e172 {

Image AbstractGraphicsProvider::imageFromData(SharedContainer::DataPtr data,
                                              std::size_t w,
                                              std::size_t h) const
{
    return Image::createImage(
        data,
        this,
        w,
        h,
        [this](e172::Image::DataPtr d) { destructImage(d); },
        [this](e172::Image::DataPtr ptr) -> e172::Image::Ptr { return imageBitMap(ptr); },
        [this](Image::DataPtr ptr, const std::string &path) { return saveImage(ptr, path); },
        [this](Image::DataPtr ptr0,
               Image::DataPtr ptr1,
               int x,
               int y,
               std::size_t &w,
               std::size_t &h) { return blitImages(ptr0, ptr1, x, y, w, h); },
        [this](e172::Image::DataPtr d, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h)
            -> e172::Image::DataPtr { return imageFragment(d, x, y, w, h); },
        [this](Image::DataPtr ptr, std::uint64_t code) { return transformImage(ptr, code); });
}

void AbstractGraphicsProvider::installParentToRenderer(AbstractRenderer &renderer) const
{
    renderer.m_provider = this;
    renderer.m_isValid = true;
}

} // namespace e172
