// Copyright 2023 Borys Boiko

#include "systeminfo.h"

#ifdef __unix__
#include <fstream>
#endif

#include <iostream>
#include <string>

namespace e172::sysinfo {

Either<MemotyUsageError, MemotyUsage> memoryUsage()
{
#ifdef __unix__
    std::ifstream stream("/proc/self/status");
    if (!stream.is_open()) {
        return Left(MemotyUsageError::FailedToReadProcInfo);
    }

    std::string line;
    std::cout << "-----/proc/self/status:" << std::endl;
    while (std::getline(stream, line)) {
        std::cout << "  -: " << line << std::endl;
        std::istringstream iss(line);
        int a, b;
        if (!(iss >> a >> b)) {
            break;
        } // error

        // process pair (a,b)
    }
    return Right(MemotyUsage{.rss = 0, .vm = 0});
/*
    FILE *procfile = fopen("/proc/self/status", "r");

    static constexpr std::size_t BufSize = 8192;
    char buffer[BufSize];
    fread(buffer, sizeof(char), BufSize, procfile);
    fclose(procfile);

    bool found_vmrss = false;
    bool found_vmsize = false;
    char* search_result;

    char delims[] = "\n";
    char* line = strtok(buffer, delims);

    while (line != NULL && (found_vmrss == false || found_vmsize == false)) {
        search_result = strstr(line, "VmRSS:");
        if (search_result != NULL) {
            sscanf(line, "%*s %ld", vmrss_kb);
            found_vmrss = true;
        }

        search_result = strstr(line, "VmSize:");
        if (search_result != NULL) {
            sscanf(line, "%*s %ld", vmsize_kb);
            found_vmsize = true;
        }
        line = strtok(NULL, delims);
    }
    return found_vmrss && found_vmsize;
*/
#else
    (void)vmrss_kb;
    (void)vmsize_kb;
    return false;
#endif
}

} // namespace e172::sysinfo
