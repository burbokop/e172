// Copyright 2023 Borys Boiko

#pragma once

#include <src/sharedcontainer.h>

namespace e172 {

class AudioSample : public e172::SharedContainer {
    friend class AbstractAudioProvider;
    static AudioSample createAudioSample(DataPtr data, Ptr id, Destructor destructor);

public:
    AudioSample() = default;
};

} // namespace e172
