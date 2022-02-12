#ifndef IMAGE_H
#define IMAGE_H


#include <functional>
#include <inttypes.h>
#include "../math/vector.h"
#include "../sharedcontainer.h"

namespace e172 {
class Image : public SharedContainer {
    friend class AbstractGraphicsProvider;
    friend class AbstractRenderer;
private:
    int m_width;
    int m_height;

    typedef std::function<data_ptr(data_ptr, int, int, int&, int&)> Fragment;
    typedef std::function<data_ptr(data_ptr, uint64_t)> Transformer;
    typedef std::function<ptr(data_ptr)> BitmapGetter;
    typedef std::function<bool(data_ptr, const std::string&)> Saver;
    typedef std::function<data_ptr(data_ptr, data_ptr, int, int, int&, int&)> Bliter;

    Transformer m_transformer;
    Fragment m_fragment;
    BitmapGetter m_bitmapGetter;
    Saver m_saver;
    Bliter m_bliter;

    static Image newImage(data_ptr data,
            ptr id,
            int width,
            int height,
            Destructor destructor,
            BitmapGetter bitmapGetter,
            Saver saver,
            Bliter bliter = Bliter(),
            Fragment fragment = Fragment(),
            Transformer transformer = Transformer()            
            );

    void __detach();
public:
    Image();
    Image transformed(uint64_t transformation) const;
    Image fragment(int x, int y, int w, int h) const;
    bool save(const std::string &path) const;
    Image blit(const Image& term, int x, int y) const;
    inline Image operator+(const Image& term) const { return blit(term, 0, 0); };

    ptr bitmap();

    int width() const;
    int height() const;

    Vector size() const;
};


}


#endif // IMAGE_H
