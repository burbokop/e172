// Copyright 2023 Borys Boiko

#pragma once

#include "../smartenum.h"
#include "either.h"
#include <cstdint>

namespace e172::sysinfo {

struct MemotyUsage
{
    /**
     * @brief rss - kilobytes
     */
    std::int64_t rss;

    /**
     * @brief vm - kilobytes
     */
    std::int64_t vm;
};

e172_enum_class(MemotyUsageError, FailedToReadProcInfo);

/**
 * @brief memoryUsage
 * @return 
 */
Either<MemotyUsageError, MemotyUsage> memoryUsage();

} // namespace e172::sysinfo
