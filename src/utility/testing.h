#ifndef TESTING_H
#define TESTING_H

#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

namespace e172 {
class Testing {
public:
    static std::vector<double> compare_test(size_t count, const std::function<void()>& f0, const std::function<void()>& f1, bool verbose = false);

    template<typename T, typename Iterator>
    static std::vector<double> compare_test(Iterator begin, Iterator end, const std::function<void(const T&)>& f0, const std::function<void(const T&)>& f1, bool verbose = false) {
        std::list<double> result;
        for(Iterator it = begin; it != end; ++it) {
            const auto t0 = std::chrono::system_clock::now();
            f0(*it);
            const auto t1 = std::chrono::system_clock::now();
            f1(*it);
            const auto t2 = std::chrono::system_clock::now();
            const auto c = (t2 - t1).count();
            if(c != 0) {
                result.push_back(double((t1 - t0).count()) / double(c));
            } else {
                result.push_back(std::numeric_limits<double>::max());
            }
            if(verbose) std::cout << *it << ":" << result.back() << "\n";
        }
        return std::vector<double>(result.begin(), result.end());
    }

    template<typename T>
    static std::vector<T> make_set(size_t first, T max, const std::function<T(size_t)> &f) {
        std::list<T> result;
        for(size_t i = first;; ++i) {
            result.push_back(f(i));
            if(max < result.back()) {
                break;
            }
        }
        return std::vector<T>(result.begin(), result.end());
    }
};
}
#endif // TESTING_H
