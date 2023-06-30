// Copyright 2023 Borys Boiko

#pragma once

#include "../utility/package.h"
#include <cstdint>

namespace e172 {

using PackedEntityId = std::uint64_t;
using PackedClientId = std::uint16_t;

/**
 * @brief The GamePackageType enum contains all package types used by engine
 */
enum class GamePackageType : PackageType {
    Init = 0,
    AddEntity,
    AddLoadableEntity,
    RemoveEntity,
    SyncEntity,
    Event,

    /**
     * UserType - for used defined packages
     * Example:
     * ```
     * enum MyPackageType : std::underlying_type<e172::GamePackageType>::type {
     *     MyType0 = e172::GamePackageType::UserType,
     *     MyType1,
     *     ...
     * }
     * ```
     */
    UserType = 0x1000
};

inline PackageType operator~(GamePackageType type)
{
    return PackageType(type);
}

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
