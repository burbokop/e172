// Copyright 2023 Borys Boiko

#pragma once

#include "../math/vector.h"
#include "../sharedcontainer.h"
#include <functional>
#include <inttypes.h>
#include <string>

namespace e172 {

class Image : public SharedContainer {
    friend class AbstractGraphicsProvider;
    friend class AbstractRenderer;
public:
    Image() = default;
    Image transformed(uint64_t transformation) const;
    Image fragment(std::size_t x, std::size_t y, std::size_t w, std::size_t h) const;
    bool save(const std::string &path) const;
    Image blit(const Image& term, int x, int y) const;
    Image operator+(const Image &term) const { return blit(term, 0, 0); }

    Ptr bitmap();

    std::size_t width() const { return m_width; }
    std::size_t height() const { return m_height; }
    Vector<std::size_t> size() const { return {m_width, m_height}; }

private:
    using Fragment
        = std::function<DataPtr(DataPtr, std::size_t, std::size_t, std::size_t &, std::size_t &)>;
    using Transformer = std::function<DataPtr(DataPtr, std::uint64_t)>;
    using BitmapGetter = std::function<Ptr(DataPtr)>;
    using Saver = std::function<bool(DataPtr, const std::string &)>;
    using Bliter = std::function<DataPtr(DataPtr, DataPtr, int, int, std::size_t &, std::size_t &)>;

    static Image createImage(DataPtr data,
                             Ptr provider,
                             std::size_t width,
                             std::size_t height,
                             Destructor destructor,
                             BitmapGetter bitmapGetter,
                             Saver saver,
                             Bliter bliter = Bliter(),
                             Fragment fragment = Fragment(),
                             Transformer transformer = Transformer());

    void detach();

private:
    std::size_t m_width;
    std::size_t m_height;
    Transformer m_transformer;
    Fragment m_fragment;
    BitmapGetter m_bitmapGetter;
    Saver m_saver;
    Bliter m_bliter;
};

} // namespace e172
