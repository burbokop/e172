#include "testing.h"

#include "../additional.h"

std::vector<double> e172::Testing::compare_test(size_t count, const std::function<void ()> &f0, const std::function<void ()> &f1, bool verbose) {
    std::vector<double> result(count);
    for(size_t i = 0; i < count; ++i) {
        const auto t0 = std::chrono::system_clock::now();
        f0();
        const auto t1 = std::chrono::system_clock::now();
        f1();
        const auto t2 = std::chrono::system_clock::now();
        const auto c = (t2 - t1).count();
        if(c != 0) {
            result[i] = (double((t1 - t0).count()) / double(c));
        } else {
            result[i] = std::numeric_limits<double>::max();
        }
        if(verbose) std::cout << e172::Additional::toFixedSizeString(i) << ":" << result[i] << "\n";
    }
    return result;
}
