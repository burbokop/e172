// Copyright 2023 Borys Boiko

#pragma once

#include <complex>
#include <functional>
#include <limits>
#include <map>
#include <utility>

namespace std {
class thread;
}

namespace e172 {

template<typename T>
using Complex = std::complex<T>;

template<typename T>
using ComplexFunction = std::function<Complex<T>(const Complex<T> &)>;

template<typename T>
using MatrixFiller = std::function<void(size_t, size_t, T*)>;

class Math {
public:
    static constexpr double Pi = 3.14159265358979323846;

    template<typename T>
    static T map(T value, T inMin, T inMax, T outMin, T outMax)
        requires std::is_arithmetic<T>::value
    {
        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    static bool cmpf(float a, float b, float epsilon = 0.00005f);
    static bool cmpf(double a, double b, double epsilon = 0.00005);

    static double sin(double angle);
    static double cos(double angle);
    static double acos(double value);
    static double sqrt(double value);

    template<typename T>
    static T mod(T a, T b)
        requires std::is_integral<T>::value
    {
        return a % b;
    }

    static float mod(float, float);
    static double mod(double, double);
    static long double mod(long double, long double);

    template<typename T>
    static T sqr(const T &x)
    {
        return x * x;
    }

    template<typename T>
    static T sigm(const T &x)
    {
        return T(1) / (T(1) + std::exp(-x));
    }

    template<typename T>
    static inline T sgn(const T &x) {
        const auto mod = std::abs(x);
        return (mod == 0) ? T() : x / mod;
    }

    static double constrainRadians(double value);
    static bool radiansDirection(double dstAngle, double angle);
    static double radiansDistance(double angle0, double angle1);
    static double radiansDifference(double angle1, double angle2);

#ifdef E172_USE_DEGREES_TRANSFORMS
    static double constrainDegrees(double value);
    static bool degreesDirection(double dstAngle, double angle);
    static double degreesDistance(double angle1, double angle2);
    static double degreesDifference(double angle1, double angle2);
#endif

    template<typename Container>
    static typename Container::value_type average(const Container &c)
    {
        if (c.size() <= 0)
            return 0;

        size_t i = 0;
        typename Container::value_type sum = 0;
        for (const auto &cc : c) {
            sum += cc;
            ++i;
        }
        return sum / i;
    }

    struct NullFloat
    {};

    static constexpr NullFloat null = NullFloat{};

    friend bool operator==(double value, const NullFloat &) { return Math::cmpf(value, 0); }
    friend bool operator==(const NullFloat &, double value) { return Math::cmpf(value, 0); }
    friend bool operator!=(double value, const NullFloat &) { return !Math::cmpf(value, 0); }
    friend bool operator!=(const NullFloat &, double value) { return !Math::cmpf(value, 0); }

    static double topLimitedFunction(double x);

    template<typename T>
    static size_t fractalLevel(const Complex<T> &c,
                               size_t limit = 256,
                               const ComplexFunction<T> &f = Math::sqr<Complex<T>>)
    {
        e172::Complex<T> x = {0, 0};
        while (std::abs(x) < 2) {
            x = f(x) + c;
            if (limit-- <= 0) {
                return 0;
            }
        }
        return limit;
    }

    template<typename T>
    static T fractalLevel(size_t x,
                          size_t y,
                          size_t w,
                          size_t h,
                          size_t limit = 256,
                          const ComplexFunction<T> &f = Math::sqr<Complex<T>>)
    {
        const auto real = (static_cast<double>(x) / static_cast<double>(w) - 0.5) * 4;
        const auto imag = (static_cast<double>(y) / static_cast<double>(h) - 0.5) * 4;
        return fractalLevel({real, imag}, limit, f) / static_cast<double>(limit);
    }

    template<typename T>
    static void writeFractal(size_t w,
                             size_t h,
                             size_t maxLevel,
                             T mask,
                             T *ptr,
                             const ComplexFunction<T> &f = Math::sqr<Complex<T>>)
    {
        if (maxLevel <= 0 || w <= 0 || h <= 0)
            return;
        for (size_t y = 0; y < h; ++y) {
            for (size_t x = 0; x < w; ++x) {
                ptr[(y * w) + x] = mask * fractalLevel(x, y, w, h, maxLevel, f);
            }
        }
    }

    static void concurentInitMatrix(
        size_t w, size_t h, const std::function<void(const std::pair<size_t, size_t> &)> &function);

    template<typename T>
    static void concurentWriteFractal(size_t w,
                                      size_t h,
                                      size_t maxLevel,
                                      T mask,
                                      T *ptr,
                                      const ComplexFunction<T> &f = Math::sqr<Complex<T>>)
    {
        if (maxLevel <= 0 || w <= 0 || h <= 0)
            return;

        concurentInitMatrix(w, h, [w, h, maxLevel, mask, ptr, f](const std::pair<size_t, size_t> &p) {
            ptr[(p.second * w) + p.first] = mask * fractalLevel(p.first, p.second, w, h, maxLevel, f);
        });
    }

    template<typename T>
    static MatrixFiller<T> fractal(size_t limit,
                                   T mask,
                                   const ComplexFunction<T> &f = Math::sqr<Complex<T>>,
                                   bool concurent = true)
    {
        if (concurent) {
            return [limit, mask, f](size_t w, size_t h, T *ptr) {
                e172::Math::concurentWriteFractal<T>(w, h, limit, mask, ptr, f);
            };
        } else {
            return [limit, mask, f](size_t w, size_t h, T *ptr) {
                e172::Math::writeFractal<T>(w, h, limit, mask, ptr, f);
            };
        }
    }

    template<typename T>
    static MatrixFiller<T> filler(const T& value) {
        return [value](size_t w, size_t h, T *ptr) {
            for (size_t y = 0; y < h; ++y) {
                for (size_t x = 0; x < w; ++x) {
                    ptr[(y * w) + x] = value;
                }
            }
        };
    }

    template<typename T>
    static void randInit(T *array, size_t size, double coeficient, const T &value) {
        std::srand(clock());
        for (size_t i = 0; i < size; ++i) {
            const auto r = (static_cast<double>(std::rand())
                            / static_cast<double>(
                                std::numeric_limits<decltype(std::rand())>::max()));
            if (r < coeficient) {
                array[i] = value;
            }
        }
    }

    template<typename T>
    static T wholePart(const T &value, const T &divider)
    {
        if (std::is_integral<T>::value) {
            return static_cast<T>(value / divider) * divider;
        } else {
            return std::floor(value / divider) * divider;
        }
    }

private:
    static std::map<double, double> s_sinCache;
    static std::map<double, double> s_cosCache;
    static std::map<double, double> s_arccosCache;
    static std::map<double, double> s_sqrtCache;
    static const double s_acosRoundLevel;
    static const double s_acosInvRoundLevel;
    static const double s_sqrtRoundLevel;
    static const double s_sqrtInvRoundLevel;
};

} // namespace e172
