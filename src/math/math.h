#ifndef MATH_H
#define MATH_H

#include <map>
#include <complex>
#include <functional>
#include <list>
#include <queue>
#include <type_traits>

namespace std {
    class thread;
}

namespace e172 {

typedef std::complex<double> Complex;

typedef std::function<Complex(const Complex&)> ComplexFunction;

template<typename T>
using MatrixFiller = std::function<void(size_t, size_t, T*)>;

class Math {
private:
    static std::map<double, double> sinCache;
    static std::map<double, double> cosCache;

    static std::map<double, double> arccosCache;
    static std::map<double, double> sqrtCache;

    static const double ACOS_ROUND_LEVEL;
    static const double ACOS_INV_ROUND_LEVEL;

    static const double SQRT_ROUND_LEVEL;
    static const double SQRT_INV_ROUND_LEVEL;


public:

    static constexpr double Pi = 3.14159265358979323846;

    template<typename A, typename B, typename C, typename D, typename E>
    static int map(const A &value, const B &inMin, const C &inMax, const D &outMin, const E &outMax) {
        return (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    static bool cmpf(float a, float b, float epsilon = 0.00005f);
    static bool cmpf(double a, double b, double epsilon = 0.00005);

    static double sin(double angle);
    static double cos(double angle);
    static double acos(double value);
    static double sqrt(double value);

    template<typename T>
    static inline T sqr(const T &x) { return x * x; }

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

    static double map(double value, double inMin, double inMax, double outMin, double outMax);

    template<typename Container>
    static typename Container::value_type average(const Container &c) {
        if(c.size() <= 0)
            return 0;

        size_t i = 0;
        typename Container::value_type sum = 0;
        for(const auto& cc : c) {
            sum += cc;
            ++i;
        }
        return sum / i;
    }

    struct null_float_t {};
    static constexpr null_float_t null = null_float_t();

    friend bool operator ==(double value, const null_float_t&) { return Math::cmpf(value, 0); }
    friend bool operator ==(const null_float_t&, double value) { return Math::cmpf(value, 0); }
    friend bool operator !=(double value, const null_float_t&) { return !Math::cmpf(value, 0); }
    friend bool operator !=(const null_float_t&, double value) { return !Math::cmpf(value, 0); }

    static double topLimitedFunction(double x);

    static size_t fractalLevel(const e172::Complex &c, size_t limit = 256, const ComplexFunction& f = Math::sqr<Complex>);
    static double fractalLevel(size_t x, size_t y, size_t w, size_t h, size_t limit = 256, const ComplexFunction& f = Math::sqr<Complex>);


    template <typename T>
    inline static void writeFractal(size_t w, size_t h, size_t maxLevel, T mask, T *ptr, const ComplexFunction& f = Math::sqr<Complex>) {
        if(maxLevel <= 0 || w <= 0 || h <= 0)
            return;
        for(size_t y = 0; y < h; ++y) {
            for(size_t x = 0; x < w; ++x) {
                ptr[(y * w) + x] = mask * fractalLevel(x, y, w, h, maxLevel, f);
            }
        }
    }

    static void concurentInitMatrix(size_t w, size_t h, const std::function<void(const std::pair<size_t, size_t>&)>&function);

    template <typename T>
    inline static void concurentWriteFractal(size_t w, size_t h, size_t maxLevel, T mask, T *ptr, const ComplexFunction& f = Math::sqr<Complex>) {
        if(maxLevel <= 0 || w <= 0 || h <= 0)
            return;

        concurentInitMatrix(w, h, [w, h, maxLevel, mask, ptr, f](const std::pair<size_t, size_t>& p){
            ptr[(p.second * w) + p.first] = mask * fractalLevel(p.first, p.second, w, h, maxLevel, f);
        });
    }


    template<typename T>
    static MatrixFiller<T> fractal(size_t limit, T mask, const ComplexFunction& f = Math::sqr<Complex>, bool concurent = true) {
        if(concurent) {
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
        return [value](size_t w, size_t h, T* ptr) {
            for(size_t y = 0; y < h; ++y) {
                for(size_t x = 0; x < w; ++x) {
                    ptr[(y * w) + x] = value;
                }
            }
        };
    }

    template<typename T>
    static void randInit(T *array, size_t size, double coeficient, const T &value) {
        std::srand(clock());
        for(size_t i = 0; i < size; ++i) {
            const auto r = (double(std::rand()) / double(std::numeric_limits<decltype (std::rand())>::max()));
            if (r < coeficient) {
                array[i] = value;
            }
        }
    }
};



}

#endif // MATH_H
