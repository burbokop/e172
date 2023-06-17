// Copyright 2023 Borys Boiko

#pragma once

#include "src/utility/package.h"
#include <cstdint>

namespace e172 {

using PackedEntityId = std::uint64_t;
using PackedClientId = std::uint16_t;

/**
 * @brief The GamePackageType enum contains all package types used by engine
 */
enum class GamePackageType : PackageType {
    Init = 0,
    AddEntity = 1,
    RemoveEntity = 2,
    SyncEntity = 3,
    Event = 4,
    /**
     * UserType - for used defined packages
     * Example:
     * ```
     * enum MyPackageType {
     *     MyType0 = GamePackageType::UserType,
     *     MyType1,
     *     ...
     * }
     * ```
     */
    UserType = 0x1000
};

/**
 * @brief The Statistics class provides information about bytes received and transmitted per second
 * e172::GameServer & e172::GameClient provide instance of this
 */
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
