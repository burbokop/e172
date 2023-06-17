// Copyright 2023 Borys Boiko

#include "abstractaudioprovider.h"

namespace e172 {

AudioSample AbstractAudioProvider::createAudioSample(AudioSample::DataPtr data,
                                                     AudioSample::Ptr id,
                                                     SharedContainer::Destructor destructor)
{
    return AudioSample::createAudioSample(data, id, destructor);
}

AudioChannel AbstractAudioProvider::createAudioChannel(AudioChannel::DataPtr data,
                                                       AudioChannel::Ptr id,
                                                       SharedContainer::Destructor destructor,
                                                       AudioChannel::VolumeSetter volumeSetter,
                                                       AudioChannel::Play play,
                                                       AudioChannel::IsPlaying isPalying,
                                                       AudioChannel::Pause pause)
{
    return AudioChannel::createAudioChannel(data,
                                            id,
                                            destructor,
                                            volumeSetter,
                                            play,
                                            isPalying,
                                            pause);
}

} // namespace e172
