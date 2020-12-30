#include "abstractgraphicsprovider.h"
#include "../functional/metafunction.h"

namespace e172 {

std::vector<std::string> AbstractGraphicsProvider::args() const {
    return m_args;
}

Image AbstractGraphicsProvider::imageFromData(SharedContainer::data_ptr data, int w, int h) const {
    return Image::newImage(data, this, w, h, [this](e172::Image::data_ptr d) {
        destructImage(d);
    }, [this](e172::Image::data_ptr ptr) -> e172::Image::ptr {
        return imageBitMap(ptr);
    }, [this](Image::data_ptr ptr, const std::string& path){
        return saveImage(ptr, path);
    }, [this](e172::Image::data_ptr d, int x, int y, int &w, int &h) -> e172::Image::data_ptr {
        return imageFragment(d, x, y, w, h);
    }, [this](Image::data_ptr ptr, uint64_t code){
        return transformImage(ptr, code);
    });
}

void AbstractGraphicsProvider::installParentToRenderer(AbstractRenderer *renderer) {
    renderer->m_provider = this;
    renderer->m_isValid = true;
}

AbstractGraphicsProvider::AbstractGraphicsProvider(const std::vector<std::string> &args) {
    m_args = args;
}

AbstractGraphicsProvider::~AbstractGraphicsProvider() {}
}
