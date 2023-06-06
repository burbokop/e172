#pragma once

#include "src/utility/package.h"
#include <cstdint>
#include <exception>

namespace e172 {

using PackedEntityId = std::uint64_t;
using PackedPlayerId = std::uint16_t;

enum class GamePackageType : PackageType { SyncEntity = 0, Event = 1, UserType = 0x1000 };

} // namespace e172
