#ifndef AUDIOCHANNEL_H
#define AUDIOCHANNEL_H

#include <map>

#include "audiosample.h"

namespace e172 {

class AudioChannel : public SharedContainer {
    friend class AbstractAudioProvider;
public:
    static const int Infinitely;
    typedef std::function<void(data_ptr, double)> VolumeSetter;
    typedef std::function<void(data_ptr, const AudioSample&, int)> Play;
    typedef std::function<bool(data_ptr)> IsPlaying;
    typedef std::function<void(data_ptr)> Pause;
    static AudioChannel newAudioChannel(
            data_ptr data,
            ptr id,
            Destructor destructor,
            VolumeSetter volumeSetter,
            Play play,
            IsPlaying isPalying,
            Pause pause
            );
private:
    VolumeSetter m_volumeSetter;
    Play m_play;
    IsPlaying m_isPalying;
    Pause m_pause;

    double m_volume = 1;
    double m_distance_volume = 1;

    double m_maxDistance = 1024;
    double m_minDistance = 128;
public:
    AudioChannel();

    void setVolume(double volume);
    void setDistance(double distance);

    void play(const AudioSample &sample, int loops = 1);
    bool isPlaying();
    void stop();

    void setMaxDistance(double maxDistance);
    void setMinDistance(double minDistance);
    double maxDistance() const;
    double minDistance() const;
};

}

#endif // AUDIOCHANNEL_H
