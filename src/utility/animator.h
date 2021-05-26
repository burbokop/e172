#ifndef ANIMATOR_H
#define ANIMATOR_H


#include <vector>
#include <src/math/vector.h>
#include <src/time/elapsedtimer.h>
#include <src/graphics/image.h>

namespace e172 {

class AbstractRenderer;
class Animator {
public:
    enum {
        PingPong,
        Loop,
        OneFrame,
        Froze,
        NotRender,
        ToTheFrame,
        Inactive
    };
private:
    e172::Vector m_position;
    double m_angle;
    double m_zoom;

    unsigned m_mode = NotRender;
    unsigned m_defaultMode = Inactive;

    e172::ElapsedTimer m_timer = e172::ElapsedTimer(100);
    e172::Image m_origin;
    std::vector<e172::Image> m_frames;
    int m_frameCount;
    int m_trackCount;
    int m_currentFrameIndex;
    int m_currentTrackIndex;
    bool m_isValid = false;

    static inline int nextId = 0;
    int m_id = nextId++;
public:
    Animator();
    Animator(const e172::Image &origin, int frames = 1, int tracks = 1);
    void play(unsigned mode);
    void setDefaultMode(unsigned value);
    void setPosition(const Vector &position);
    void setAngle(double angle);
    void setZoom(double zoom);

    // Entity interface
public:
    Vector render(e172::AbstractRenderer *renderer);


    friend bool operator ==(const Animator& anim0, const Animator& anim1);
    bool isValid() const;
    int frameCount() const;
    int trackCount() const;
};

}

#endif // ANIMATOR_H
