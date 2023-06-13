#include "abstractgraphicsprovider.h"

namespace e172 {

std::vector<std::string> AbstractGraphicsProvider::args() const {
    return m_args;
}

Image AbstractGraphicsProvider::imageFromData(SharedContainer::data_ptr data, std::size_t w, std::size_t h) const {
    return Image::newImage(
        data,
        this,
        w,
        h,
        [this](e172::Image::data_ptr d) { destructImage(d); },
        [this](e172::Image::data_ptr ptr) -> e172::Image::ptr { return imageBitMap(ptr); },
        [this](Image::data_ptr ptr, const std::string &path) { return saveImage(ptr, path); },
        [this](Image::data_ptr ptr0,
               Image::data_ptr ptr1,
               int x,
               int y,
               std::size_t &w,
               std::size_t &h) { return blitImages(ptr0, ptr1, x, y, w, h); },
        [this](e172::Image::data_ptr d, std::size_t x, std::size_t y, std::size_t &w, std::size_t &h)
            -> e172::Image::data_ptr { return imageFragment(d, x, y, w, h); },
        [this](Image::data_ptr ptr, std::uint64_t code) { return transformImage(ptr, code); });
}

void AbstractGraphicsProvider::installParentToRenderer(AbstractRenderer *renderer) {
    renderer->m_provider = this;
    renderer->m_isValid = true;
}

} // namespace e172
