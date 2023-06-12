#pragma once

#include "../math/vector.h"
#include "../sharedcontainer.h"
#include <functional>
#include <inttypes.h>

namespace e172 {

class Image : public SharedContainer {
    friend class AbstractGraphicsProvider;
    friend class AbstractRenderer;
private:
    std::size_t m_width;
    std::size_t m_height;

    typedef std::function<data_ptr(data_ptr, std::size_t, std::size_t, std::size_t &, std::size_t &)>
        Fragment;
    typedef std::function<data_ptr(data_ptr, uint64_t)> Transformer;
    typedef std::function<ptr(data_ptr)> BitmapGetter;
    typedef std::function<bool(data_ptr, const std::string&)> Saver;
    typedef std::function<data_ptr(data_ptr, data_ptr, int, int, std::size_t &, std::size_t &)>
        Bliter;

    Transformer m_transformer;
    Fragment m_fragment;
    BitmapGetter m_bitmapGetter;
    Saver m_saver;
    Bliter m_bliter;

    static Image newImage(data_ptr data,
                          ptr provider,
                          std::size_t width,
                          std::size_t height,
                          Destructor destructor,
                          BitmapGetter bitmapGetter,
                          Saver saver,
                          Bliter bliter = Bliter(),
                          Fragment fragment = Fragment(),
                          Transformer transformer = Transformer());

    void __detach();
public:
    Image();
    Image transformed(uint64_t transformation) const;
    Image fragment(std::size_t x, std::size_t y, std::size_t w, std::size_t h) const;
    bool save(const std::string &path) const;
    Image blit(const Image& term, int x, int y) const;
    inline Image operator+(const Image& term) const { return blit(term, 0, 0); };

    ptr bitmap();

    std::size_t width() const { return m_width; }
    std::size_t height() const { return m_height; }

    Vector<std::size_t> size() const { return {m_width, m_height}; }
};

}
