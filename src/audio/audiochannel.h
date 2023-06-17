// Copyright 2023 Borys Boiko

#pragma once

#include <map>
#include "audiosample.h"

namespace e172 {

class AudioChannel : public SharedContainer {
    friend class AbstractAudioProvider;
public:
    static const int Infinitely;

    using VolumeSetter = std::function<void(DataPtr, double)>;
    using Play = std::function<void(DataPtr, const AudioSample &, int)>;
    using IsPlaying = std::function<bool(DataPtr)>;
    using Pause = std::function<void(DataPtr)>;

    static AudioChannel createAudioChannel(DataPtr data,
                                           Ptr id,
                                           Destructor destructor,
                                           VolumeSetter volumeSetter,
                                           Play play,
                                           IsPlaying isPalying,
                                           Pause pause);

    AudioChannel() = default;

    void setVolume(double volume) { m_volume = volume; }
    void setDistance(double distance);

    void play(const AudioSample &sample, int loops = 1);
    bool isPlaying();
    void stop();

    void setMaxDistance(double maxDistance) { m_maxDistance = maxDistance; }
    void setMinDistance(double minDistance) { m_minDistance = minDistance; }
    double maxDistance() const { return m_maxDistance; }
    double minDistance() const { return m_minDistance; }

private:
    VolumeSetter m_volumeSetter;
    Play m_play;
    IsPlaying m_isPalying;
    Pause m_pause;
    double m_volume = 1;
    double m_distance_volume = 1;
    double m_maxDistance = 1024;
    double m_minDistance = 128;
};

} // namespace e172
