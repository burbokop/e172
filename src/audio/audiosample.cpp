// Copyright 2023 Borys Boiko

#include "audiosample.h"

namespace e172 {

AudioSample AudioSample::createAudioSample(e172::SharedContainer::DataPtr data,
                                           e172::SharedContainer::Ptr id,
                                           Destructor destructor)
{
    return createSharedContainer<AudioSample>(data, id, destructor);
}

} // namespace e172
