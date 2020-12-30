#ifndef IMAGE_H
#define IMAGE_H


#include <functional>
#include <inttypes.h>
#include <src/sharedcontainer.h>

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

    Transformer m_transformer;
    Fragment m_fragment;
    BitmapGetter m_bitmapGetter;
    Saver m_saver;

    static Image newImage(data_ptr data,
            ptr id,
            int width,
            int height,
            Destructor destructor,
            BitmapGetter bitmapGetter,
            Saver saver,
            Fragment fragment = Fragment(),
            Transformer transformer = Transformer()
            );

    void __detach();
public:
    Image();
    Image transformed(uint64_t transformation) const;
    Image fragment(int x, int y, int w, int h) const;
    bool save(const std::string &path) const;

    ptr bitmap();

    int width() const;
    int height() const;
};


}


#endif // IMAGE_H
