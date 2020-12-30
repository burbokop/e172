#ifndef AUDIOPROVIDER_H
#define AUDIOPROVIDER_H

#include "audiochannel.h"
#include "audiosample.h"

#include <string>
namespace e172 {
class AbstractAudioProvider {
    double m_generalVolume = 1;
protected:
    static AudioSample __newAudioSample(AudioSample::data_ptr data, AudioSample::ptr id, AudioSample::Destructor destructor);
    static AudioChannel __newAudioChannel(
            AudioChannel::data_ptr data,
            AudioChannel::ptr id,
            AudioChannel::Destructor destructor,
            AudioChannel::VolumeSetter volumeSetter,
            AudioChannel::Play play,
            AudioChannel::IsPlaying isPalying,
            AudioChannel::Pause pause
            );
    template<typename T>
    static T audioSampleData(const AudioSample &sample) { return sample.casted_handle<T>()->c; }
    static AudioSample::ptr audioId(const AudioSample &sample);

public:
    AbstractAudioProvider();
    virtual AudioSample loadAudioSample(const std::string &path) = 0;
    virtual AudioChannel reserveChannel() = 0;
    virtual ~AbstractAudioProvider();
    double generalVolume() const;
    void setGeneralVolume(double generalVolume);
};
}
#endif // AUDIOPROVIDER_H
