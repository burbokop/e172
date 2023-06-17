// Copyright 2023 Borys Boiko

#pragma once

#include "audiochannel.h"
#include "audiosample.h"
#include <string>

namespace e172 {

class AbstractAudioProvider {
public:
    AbstractAudioProvider() = default;
    virtual ~AbstractAudioProvider() = default;

    double generalVolume() const { return m_generalVolume; }
    void setGeneralVolume(double generalVolume) { m_generalVolume = generalVolume; }

    virtual AudioSample loadAudioSample(const std::string &path) = 0;
    virtual AudioChannel reserveChannel() = 0;

protected:
    static AudioSample createAudioSample(AudioSample::DataPtr data,
                                         AudioSample::Ptr id,
                                         AudioSample::Destructor destructor);
    static AudioChannel createAudioChannel(AudioChannel::DataPtr data,
                                           AudioChannel::Ptr id,
                                           AudioChannel::Destructor destructor,
                                           AudioChannel::VolumeSetter volumeSetter,
                                           AudioChannel::Play play,
                                           AudioChannel::IsPlaying isPalying,
                                           AudioChannel::Pause pause);
    template<typename T>
    static T audioSampleData(const AudioSample &sample)
    {
        return sample.handleAs<T>()->c;
    }

    static AudioSample::Ptr audioId(const AudioSample &sample);

private:
    double m_generalVolume = 1;
};

} // namespace e172
