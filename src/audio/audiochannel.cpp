// Copyright 2023 Borys Boiko

#include "audiochannel.h"

#include <src/math/math.h>

namespace e172 {

const int AudioChannel::Infinitely = -1;

AudioChannel AudioChannel::createAudioChannel(SharedContainer::DataPtr data,
                                              SharedContainer::Ptr id,
                                              Destructor destructor,
                                              VolumeSetter volumeSetter,
                                              Play play,
                                              IsPlaying isPalying,
                                              Pause pause)
{
    auto result = createSharedContainer<AudioChannel>(data, id, destructor);
    result.m_play = play;
    result.m_volumeSetter = volumeSetter;
    result.m_isPalying = isPalying;
    result.m_pause = pause;
    return result;
}

void AudioChannel::play(const AudioSample &sample, int loops)
{
    if (isValid()) {
        const auto v = m_volume * m_distance_volume;
        if (m_volumeSetter)
            m_volumeSetter(data(), v);

        if (v > 0 && sample.isValid()) {
            m_play(data(), sample, loops);
        }
    }
}

bool AudioChannel::isPlaying()
{
    if (m_isPalying)
        return m_isPalying(data());

    return false;
}

void AudioChannel::stop()
{
    if (m_pause)
        m_pause(data());
}

void AudioChannel::setDistance(double distance)
{
    if (distance < m_minDistance)
        distance = m_minDistance;
    if (distance > m_maxDistance)
        distance = m_maxDistance;
    m_distance_volume = e172::Math::map<double>(distance, m_minDistance, m_maxDistance, 1, 0);
}

} // namespace e172
