#pragma once

#include "src/utility/package.h"
#include <cstdint>
#include <exception>

namespace e172 {

using PackedEntityId = std::uint64_t;
using PackedPlayerId = std::uint16_t;

enum class GamePackageType : PackageType {
    AddEntity = 0,
    SyncEntity = 1,
    Event = 2,
    UserType = 0x1000
};

} // namespace e172
