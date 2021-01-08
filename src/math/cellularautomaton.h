#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H

#include <set>
#include <functional>

namespace e172 {


struct CellularAutomaton {
    typedef std::pair<std::set<size_t>, std::set<size_t>> Rule;

    static inline const Rule b3s23 = { { 3 }, { 2, 3 } };
    static inline const Rule b3s12345 = { { 3 }, { 1, 2, 3, 4, 5 } };

    static inline const Rule gameOfLife = b3s23;
    static inline const Rule maze = b3s12345;

    template<typename T>
    static size_t mooreNeighborhood(size_t x, size_t y, size_t w, size_t h, const T &matrix) {
        const auto at = [matrix, w, h](size_t x, size_t y) {
            x %= w;
            y %= h;
            return matrix[y * w + x];
        };

        return size_t(at(x + 1, y))
                + size_t(at(x + 1, y + 1))
                + size_t(at(x, y + 1))
                + size_t(at(x - 1, y + 1))
                + size_t(at(x - 1, y))
                + size_t(at(x - 1, y - 1))
                + size_t(at(x, y - 1))
                + size_t(at(x + 1, y - 1));
    }

    template <typename T>
    using Neighborhood = std::function<size_t(size_t, size_t, size_t, size_t, const T&)>;

    template<typename T>
    static size_t vonNeumannNeighborhood(size_t x, size_t y, size_t w, size_t h, const T &matrix) {
        const auto at = [matrix, w, h](size_t x, size_t y) {
            x %= w;
            y %= h;
            return matrix[y * w + x];
        };

        return size_t(at(x + 1, y))
                + size_t(at(x, y + 1))
                + size_t(at(x - 1, y))
                + size_t(at(x, y - 1));
    }

    template<typename T>
    static void proceed(size_t w, size_t h, T&& matrix, const Rule &rule = gameOfLife, const Neighborhood<T>& neighborhood = mooreNeighborhood<T>) {
        const auto at = [&matrix, w](size_t x, size_t y) -> auto& {
            return matrix[y * w + x];
        };

        std::optional<bool> m[w][h];
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                const auto c = neighborhood(x, y, w, h, matrix);
                if(at(x, y) == false) {
                    if(rule.first.contains(c)) {
                        m[x][y] = true;
                    }
                } else {
                    if(!rule.second.contains(c)) {
                        m[x][y] = false;
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
};
}

#endif // CELLULARAUTOMATON_H
