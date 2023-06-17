// Copyright 2023 Borys Boiko

#include "audioplayer.h"

#include <src/math/math.h>

bool e172::AudioPlayer::play()
{
    if (m_state == Idle || !m_waitStopPlaing) {
        m_channel.play(m_beginningSample, 1);
        m_state = Beginning;
        return true;
    }
    return false;
}

bool e172::AudioPlayer::stop() {
    if (m_state == Beginning || m_state == Loop) {
        m_channel.play(m_endingSample, 1);
        m_state = Ending;
        return true;
    }
    return false;
}

void e172::AudioPlayer::proceed()
{
    if (m_state == Beginning) {
        if (!m_channel.isPlaying()) {
            if (m_loopSample.isValid()) {
                m_channel.play(m_loopSample, e172::AudioChannel::Infinitely);
                m_state = Loop;
            } else {
                m_state = Idle;
            }
        }
    } else if (m_state == Ending) {
        if (!m_channel.isPlaying()) {
            m_state = Idle;
        }
    }
}

void e172::AudioPlayer::setVolume(double volume) {
    m_channel.setVolume(volume);
}

void e172::AudioPlayer::setDistance(double distance) {
    m_channel.setDistance(distance);
}
