#ifndef AUDIOSAMPLE_H
#define AUDIOSAMPLE_H

#include <src/sharedcontainer.h>

namespace e172 {


class AudioSample : public e172::SharedContainer {
    friend class AbstractAudioProvider;
    static AudioSample newAudioSample(data_ptr data, ptr id, Destructor destructor);

public:
    AudioSample();
};

}
#endif // AUDIOSAMPLE_H
