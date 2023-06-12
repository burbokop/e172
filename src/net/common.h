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

struct Statistics
{
    std::size_t bytesWritenPerSecond;
    std::size_t bytesReadPerSecond;
};

inline std::ostream &operator<<(std::ostream &stream, const Statistics &s)
{
    return stream << "{ bytesWritenPerSecond: " << s.bytesWritenPerSecond
                  << ", bytesReadPerSecond: " << s.bytesReadPerSecond << " }";
}

} // namespace e172
