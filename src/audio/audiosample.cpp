#include "audiosample.h"

namespace e172 {


AudioSample AudioSample::newAudioSample(e172::SharedContainer::data_ptr data, e172::SharedContainer::ptr id, Destructor destructor)
{

    return newSharedContainer<AudioSample>(data,id,destructor);
}

AudioSample::AudioSample()
{

}

}
