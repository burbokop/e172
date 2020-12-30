#include "abstractaudioprovider.h"

namespace e172 {


double AbstractAudioProvider::generalVolume() const
{
    return m_generalVolume;
}

void AbstractAudioProvider::setGeneralVolume(double generalVolume)
{
    m_generalVolume = generalVolume;
}

AudioSample AbstractAudioProvider::__newAudioSample(AudioSample::data_ptr data, AudioSample::ptr id, SharedContainer::Destructor destructor) {
    return AudioSample::newAudioSample(data, id, destructor);
}

AudioChannel AbstractAudioProvider::__newAudioChannel(AudioChannel::data_ptr data,
        AudioChannel::ptr id,
        SharedContainer::Destructor destructor,
        AudioChannel::VolumeSetter volumeSetter,
        AudioChannel::Play play,
        AudioChannel::IsPlaying isPalying,
        AudioChannel::Pause pause) {
    return AudioChannel::newAudioChannel(data, id, destructor, volumeSetter, play, isPalying, pause);
}

AbstractAudioProvider::AbstractAudioProvider()
{

}

AbstractAudioProvider::~AbstractAudioProvider() {}

}
