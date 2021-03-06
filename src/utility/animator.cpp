#include "animator.h"

#include <src/graphics/abstractrenderer.h>

namespace e172 {

void Animator::setDefaultMode(unsigned value) {
    m_defaultMode = value;
}

bool Animator::isValid() const {
    return m_isValid;
}

int Animator::frameCount() const {
    return m_frameCount;
}

int Animator::trackCount() const {
    return m_trackCount;
}

Animator::Animator() {}

Animator::Animator(const e172::Image &origin, int frames, int tracks) {
    m_angle = 0;
    m_zoom = 1;

    m_origin = origin;
    m_frameCount = frames;
    m_trackCount = tracks;
    const int frameWidth = origin.width() / frames;
    const int frameHeigth = origin.height() / tracks;

    m_currentFrameIndex = 0;
    m_currentTrackIndex = 0;

    for(int i = 0; i < m_frameCount; i++) {
        m_frames.push_back(origin.fragment(i * frameWidth, 0, frameWidth, frameHeigth));
    }

    m_isValid = m_frames.size() > 0;
}

void Animator::play(unsigned mode) {
    m_mode = mode;
}


void Animator::setPosition(const e172::Vector &position) {
    m_position = position;
}

void Animator::setAngle(double angle) {
    m_angle = angle;
}

void Animator::setZoom(double zoom) {
    m_zoom = zoom;
}

e172::Vector Animator::render(e172::AbstractRenderer *renderer) {
    if(m_isValid) {
        const auto currentFrame = m_frames[static_cast<unsigned long>(m_currentFrameIndex)];
        if (renderer != nullptr && m_mode != NotRender) {
            renderer->drawImageShifted(currentFrame, m_position, m_angle, m_zoom);
        }
        if(m_timer.check()) {
            if(m_mode == Loop) {
                m_currentFrameIndex++;
                if(m_currentFrameIndex >= m_frameCount) m_currentFrameIndex = 0;
            }
        }
        if(m_defaultMode != Inactive) m_mode = m_defaultMode;
        return e172::Vector(currentFrame.width(), currentFrame.height());
    }
    return e172::Vector();
}

bool operator ==(const Animator &anim0, const Animator &anim1) {
    return anim0.m_id == anim1.m_id;
}
}
