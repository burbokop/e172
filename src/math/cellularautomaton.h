#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H

#include <set>
#include <map>
#include <functional>
#include "../utility/matrixproxy.h"

namespace e172 {


struct CellularAutomaton {
    typedef std::pair<std::set<size_t>, std::set<size_t>> Rule;

    template <typename T>
    using ExtendedRule = std::map<T, std::pair<T, std::optional<std::set<size_t>>>>;

    static inline const Rule b3s23 = { { 3 }, { 2, 3 } };
    static inline const Rule b3s12345 = { { 3 }, { 1, 2, 3, 4, 5 } };
    static inline const Rule b1s012345678 = { { 1 }, { 0, 1, 2, 3, 4, 5, 6, 7, 8 } };

    static inline const Rule gameOfLife = b3s23;
    static inline const Rule maze = b3s12345;
    static inline const Rule replicator = b1s012345678;

    enum WireWorldCell {
        Empty = 0,
        Wire,
        Signal,
        SignalEnd
    };

    static inline const ExtendedRule<WireWorldCell> wireWorld = ExtendedRule<WireWorldCell> {
        { Wire, std::pair { Signal, std::set<size_t> { 1, 2 } } },
        { Signal, std::pair { SignalEnd, std::nullopt } },
        { SignalEnd, std::pair { Wire, std::nullopt } }
    };


    template <typename T>
    using Neighborhood = std::function<size_t(size_t, size_t, size_t, size_t, const T*, const T&)>;

    template<typename T>
    static size_t mooreNeighborhood(size_t x, size_t y, size_t w, size_t h, const T *matrix, const T &value) {
        const auto at = [matrix, w, h](size_t x, size_t y) {
            x %= w;
            y %= h;
            return matrix[y * w + x];
        };

        return size_t(at(x + 1, y) == value)
                + size_t(at(x + 1, y + 1) == value)
                + size_t(at(x, y + 1) == value)
                + size_t(at(x - 1, y + 1) == value)
                + size_t(at(x - 1, y) == value)
                + size_t(at(x - 1, y - 1) == value)
                + size_t(at(x, y - 1) == value)
                + size_t(at(x + 1, y - 1) == value);
    }

    template<typename T>
    static size_t vonNeumannNeighborhood(size_t x, size_t y, size_t w, size_t h, const T *matrix, const T &value) {
        const auto at = [matrix, w, h](size_t x, size_t y) {
            x %= w;
            y %= h;
            return matrix[y * w + x];
        };

        return size_t(at(x + 1, y) == value)
                + size_t(at(x, y + 1) == value)
                + size_t(at(x - 1, y) == value)
                + size_t(at(x, y - 1) == value);
    }

    template<typename T>
    static void proceed(size_t w, size_t h, T* matrix, const Rule &rule = gameOfLife, const Neighborhood<T>& neighborhood = mooreNeighborhood<T>) {
        const auto at = [&matrix, w](size_t x, size_t y) -> auto& {
            return matrix[y * w + x];
        };

        std::optional<T> m[w][h];
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                const auto c = neighborhood(x, y, w, h, matrix, true);
                if(at(x, y)) {
                    if(!rule.second.contains(c)) {
                        m[x][y] = false;
                    }
                } else {
                    if(rule.first.contains(c)) {
                        m[x][y] = true;
                    }
                }
            }
        }
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                if(m[x][y].has_value()) {
                    at(x, y) = m[x][y].value();
                }
            }
        }
    }

    template<typename T>
    static void proceed(size_t w, size_t h, T* matrix, const ExtendedRule<T> &rule, const Neighborhood<T>& neighborhood = mooreNeighborhood<T>) {
        const auto at = [&matrix, w](size_t x, size_t y) -> auto& {
            return matrix[y * w + x];
        };

        std::optional<T> m[w][h];
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                const auto& it = rule.find(at(x, y));
                if(it != rule.end()) {
                    if(it->second.second.has_value()) {
                        if(it->second.second.value().contains(neighborhood(x, y, w, h, matrix, it->second.first))) {
                            m[x][y] = it->second.first;
                        }
                    } else {
                        m[x][y] = it->second.first;
                    }
                }
            }
        }
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                if(m[x][y].has_value()) {
                    at(x, y) = m[x][y].value();
                }
            }
        }
    }

    template<typename T>
    static inline void proceed(MatrixProxy<T> &matrixProxy, const Rule &rule = gameOfLife, const Neighborhood<T>& neighborhood = mooreNeighborhood<T>) {
        proceed(matrixProxy.width(), matrixProxy.height(), matrixProxy.data(), rule, neighborhood);
    }

    template<typename T>
    static inline void proceed(MatrixProxy<T> &matrixProxy, const ExtendedRule<T> &rule, const Neighborhood<T>& neighborhood = mooreNeighborhood<T>) {
        proceed(matrixProxy.width(), matrixProxy.height(), matrixProxy.data(), rule, neighborhood);
    }
};
}

#endif // CELLULARAUTOMATON_H
