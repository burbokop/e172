#include "time.h"
#include <chrono>


namespace e172 {

Time::time_t Time::currentMilliseconds() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count();
}

}
