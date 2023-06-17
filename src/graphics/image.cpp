// Copyright 2023 Borys Boiko

#include "image.h"

namespace e172 {

Image Image::createImage(DataPtr data,
                         Ptr provider,
                         std::size_t width,
                         std::size_t height,
                         Destructor destructor,
                         BitmapGetter bitmapGetter,
                         Saver saver,
                         Bliter bliter,
                         Fragment fragment,
                         Transformer transformer)
{
    Image i = createSharedContainer<Image>(data, provider, destructor);
    i.m_width = width;
    i.m_height = height;
    i.m_bitmapGetter = bitmapGetter;
    i.m_saver = saver;
    i.m_transformer = transformer;
    i.m_fragment = fragment;
    i.m_bliter = bliter;
    return i;
}

Image Image::transformed(uint64_t transformation) const {
    Image result = *this;
    if (m_transformer) {
        result.setData(m_transformer(data(), transformation));
    }
    return result;
}

Image Image::fragment(std::size_t x, std::size_t y, std::size_t w, std::size_t h) const
{
    Image result = *this;
    if (m_fragment) {
        result.setData(m_fragment(data(), x, y, w, h));
        result.m_width = w;
        result.m_height = h;
    }
    return result;
}

bool Image::save(const std::string &path) const {
    if (m_saver && data()) {
        return m_saver(data(), path);
    }
    return false;
}

Image Image::blit(const Image &term, int x, int y) const {
    if (m_bliter && data()) {
        Image result = *this;
        if (!term.data())
            return result;

        result.setData(m_bliter(data(), term.data(), x, y, result.m_width, result.m_height));
        return result;
    }
    return {};
}

Image::Ptr Image::bitmap()
{
    if (m_bitmapGetter && data())
        return m_bitmapGetter(data());
    return nullptr;
}

} // namespace e172
