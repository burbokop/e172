#pragma once

#include "src/utility/package.h"
#include <cstdint>
#include <exception>

namespace e172 {

using PackedEntityId = std::uint64_t;
using PackedClientId = std::uint16_t;

enum class GamePackageType : PackageType {
    Init = 0,
    AddEntity = 1,
    RemoveEntity = 2,
    SyncEntity = 3,
    Event = 4,
    UserType = 0x1000
};

} // namespace e172
