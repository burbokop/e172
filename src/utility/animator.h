#pragma once

#include <src/graphics/image.h>
#include <src/math/vector.h>
#include <src/time/elapsedtimer.h>
#include <vector>

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

    Animator();
    Animator(const e172::Image &origin, int frames = 1, int tracks = 1);
    void play(unsigned mode);
    void setDefaultMode(unsigned value);
    void setPosition(const Vector<double> &position);
    void setAngle(double angle);
    void setZoom(double zoom);

    friend bool operator==(const Animator &anim0, const Animator &anim1);
    bool isValid() const;
    int frameCount() const;
    int trackCount() const;

    // Entity interface
public:
    Vector<double> render(e172::AbstractRenderer *renderer);

private:
    e172::Vector<double> m_position;
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

    static inline int s_nextId = 0;
    int m_id = s_nextId++;
};

} // namespace e172
