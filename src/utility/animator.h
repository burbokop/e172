// Copyright 2023 Borys Boiko

#pragma once

#include <src/graphics/image.h>
#include <src/math/vector.h>
#include <src/time/elapsedtimer.h>
#include <vector>

namespace e172 {

class AbstractRenderer;
class Animator {
public:
    enum Mode { PingPong, Loop, OneFrame, Froze, NotRender, ToTheFrame, Inactive };

    Animator() = default;
    Animator(const e172::Image &origin, int frames = 1, int tracks = 1);
    void play(Mode mode);
    void setDefaultMode(Mode value) { m_defaultMode = value; }
    void setPosition(const Vector<double> &position) { m_position = position; }
    void setAngle(double angle) { m_angle = angle; }
    void setZoom(double zoom) { m_zoom = zoom; }

    friend bool operator==(const Animator &anim0, const Animator &anim1);
    bool isValid() const { return m_isValid; }
    auto frameCount() const { return m_frameCount; }
    auto trackCount() const { return m_trackCount; }

    // Entity interface
public:
    Vector<double> render(e172::AbstractRenderer *renderer);

private:
    e172::Vector<double> m_position;
    double m_angle;
    double m_zoom;

    Mode m_mode = NotRender;
    Mode m_defaultMode = Inactive;

    e172::ElapsedTimer m_timer = e172::ElapsedTimer(100);
    e172::Image m_origin;
    std::vector<e172::Image> m_frames;
    std::size_t m_frameCount;
    std::size_t m_trackCount;
    std::size_t m_currentFrameIndex;
    std::size_t m_currentTrackIndex;
    bool m_isValid = false;

    static inline std::size_t s_nextId = 0;
    std::size_t m_id = s_nextId++;
};

} // namespace e172
