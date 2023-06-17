// Copyright 2023 Borys Boiko

#pragma once

#include <src/audio/audiochannel.h>

namespace e172 {

class AudioPlayer {
public:
    enum State {
        Idle,
        Beginning,
        Loop,
        Ending
    };

    AudioPlayer(const e172::AudioChannel channel = e172::AudioChannel(),
                const e172::AudioSample &start = e172::AudioSample(),
                const e172::AudioSample &loop = e172::AudioSample(),
                const e172::AudioSample &stop = e172::AudioSample())
        : m_channel(channel)
        , m_beginningSample(start)
        , m_loopSample(loop)
        , m_endingSample(stop)
    {}

    bool play();
    bool stop();
    void proceed();

    /**
     * @brief setVolume
     * @param volume - value in percent from 0 to 1
     */
    void setVolume(double volume);
    void setDistance(double distance);
    void setWaitStopPlaing(bool waitStopPlaing) { m_waitStopPlaing = waitStopPlaing; }

    inline friend bool operator==(const AudioPlayer &ap0, const AudioPlayer &ap1)
    {
        return ap0.m_id == ap1.m_id;
    }

    e172::AudioSample beginningSample() const { return m_beginningSample; }
    e172::AudioSample loopSample() const { return m_loopSample; }
    e172::AudioSample endingSample() const { return m_endingSample; }
    e172::AudioChannel channel() const { return m_channel; }

private:
    static inline int s_nextId = 0;

    e172::AudioChannel m_channel;
    unsigned m_state = Idle;
    bool m_waitStopPlaing = false;
    e172::AudioSample m_beginningSample;
    e172::AudioSample m_loopSample;
    e172::AudioSample m_endingSample;
    int m_id = s_nextId++;
};

} // namespace e172
