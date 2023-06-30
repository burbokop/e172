// Copyright 2023 Borys Boiko

#include "animator.h"

#include "../graphics/abstractrenderer.h"

namespace e172 {

Animator::Animator(const e172::Image &origin, int frames, int tracks)
{
    m_angle = 0;
    m_zoom = 1;

    m_origin = origin;
    m_frameCount = frames;
    m_trackCount = tracks;
    const auto frameWidth = origin.width() / frames;
    const auto frameHeigth = origin.height() / tracks;

    m_currentFrameIndex = 0;
    m_currentTrackIndex = 0;

    for (int i = 0; i < m_frameCount; i++) {
        m_frames.push_back(origin.fragment(i * frameWidth, 0, frameWidth, frameHeigth));
    }

    m_isValid = m_frames.size() > 0;
}

void Animator::play(Mode mode)
{
    m_mode = mode;
}

e172::Vector<double> Animator::render(e172::AbstractRenderer *renderer)
{
    if (m_isValid) {
        const auto currentFrame = m_frames[m_currentFrameIndex];
        if (renderer && m_mode != NotRender) {
            renderer->drawImageShifted(currentFrame, m_position, m_angle, m_zoom);
        }
        if (m_timer.check()) {
            if (m_mode == Loop) {
                m_currentFrameIndex++;
                if (m_currentFrameIndex >= m_frameCount)
                    m_currentFrameIndex = 0;
            }
        }
        if (m_defaultMode != Inactive)
            m_mode = m_defaultMode;
        return e172::Vector<std::size_t>(currentFrame.width(), currentFrame.height()).into<double>();
    }
    return e172::Vector<double>();
}

bool operator ==(const Animator &anim0, const Animator &anim1) {
    return anim0.m_id == anim1.m_id;
}

} // namespace e172
