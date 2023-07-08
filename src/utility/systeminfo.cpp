// Copyright 2023 Borys Boiko

#include "systeminfo.h"

#ifdef __unix__
#include <fstream>
#else
#include "../todo.h"
#endif

#include <string>

namespace e172::sysinfo {

Either<MemotyUsageError, MemotyUsage> memoryUsage()
{
#ifdef __unix__
    static constexpr const char *UnixProcFile = "/proc/self/status";

    const auto &&parseField = [](const std::string &key,
                                 const std::string &str) -> std::optional<std::int64_t> {
        if (str.starts_with(key)) {
            return std::stoll(str.substr(key.size(), str.size() - key.size()));
        }
        return std::nullopt;
    };

    std::ifstream stream(UnixProcFile);
    if (!stream.is_open()) {
        return Left(MemotyUsageError::FailedToReadProcInfo);
    }

    std::optional<std::int64_t> rss;
    std::optional<std::int64_t> vm;
    std::string line;
    try {
        while (std::getline(stream, line)) {
            if (const auto val = parseField("VmRSS:", line)) {
                rss = val;
            }
            if (const auto val = parseField("VmSize:", line)) {
                vm = val;
            }
            if (rss && vm)
                break;
        }
    } catch (const std::invalid_argument &) {
        return Left(MemotyUsageError::FailedToParseProcInfo);
    } catch (const std::out_of_range &) {
        return Left(MemotyUsageError::FailedToParseProcInfo);
    }

    if (rss && vm) {
        return Right(MemotyUsage{.rss = *rss, .vm = *vm});
    } else {
        return Left(MemotyUsageError::MemUsageDataNotFound);
    }
#else
    todo();
#endif
}

} // namespace e172::sysinfo
