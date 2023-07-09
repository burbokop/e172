// Copyright 2023 Borys Boiko

#pragma once

#include "buffer.h"

namespace e172 {

/**
 * Load resource packed by `ld`
 * @note use it outside of any namespace
 */
#define e172_load_resource(x) \
    ([]() { \
        extern const e172::Byte _binary_##x##_start, _binary_##x##_end; \
        return e172::ByteRange(&_binary_##x##_start, &_binary_##x##_end); \
    })()

} // namespace e172
