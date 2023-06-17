// Copyright 2023 Borys Boiko

#include "math.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <execution>
#include <vector>

#define INV_PI 180 / M_PI
#define NOTINV_PI M_PI / 180

std::map<double, double> e172::Math::s_sinCache;
std::map<double, double> e172::Math::s_cosCache;

std::map<double, double> e172::Math::s_arccosCache;
std::map<double, double> e172::Math::s_sqrtCache;

const double e172::Math::s_acosRoundLevel = 1000;
const double e172::Math::s_acosInvRoundLevel = 1 / s_acosRoundLevel;

const double e172::Math::s_sqrtRoundLevel = 1000;
const double e172::Math::s_sqrtInvRoundLevel = 1 / s_sqrtRoundLevel;

bool e172::Math::cmpf(float a, float b, float epsilon)
{
    return (fabsf(a - b) < epsilon);
}

bool e172::Math::cmpf(double a, double b, double epsilon) {
    return (fabs(a - b) < epsilon);
}

double e172::Math::sin(double angle) {
    if (s_sinCache.find(angle) == s_sinCache.end()) {
        const double result = std::sin(angle);
        s_sinCache[angle] = result;
        return result;
    } else {
        return s_sinCache[angle];
    }
}


double e172::Math::cos(double angle) {
    if (s_cosCache.find(angle) == s_cosCache.end()) {
        const double result = std::cos(angle);
        s_cosCache[angle] = result;
        return result;
    } else {
        return s_cosCache[angle];
    }
}

double e172::Math::acos(double value) {
    value = std::round(value * s_acosRoundLevel) * s_acosInvRoundLevel;
    if (s_arccosCache.find(value) == s_arccosCache.end()) {
        const double result = std::acos(value);
        s_arccosCache[value] = result;
        return result;
    } else {
        return s_arccosCache[value];
    }
}

double e172::Math::sqrt(double value) {
    value = std::round(value * s_sqrtRoundLevel) * s_sqrtInvRoundLevel;
    if (s_sqrtCache.find(value) == s_sqrtCache.end()) {
        const double result = std::sqrt(value);
        s_sqrtCache[value] = result;
        return result;
    } else {
        return s_sqrtCache[value];
    }
}

double e172::Math::constrainRadians(double value) {
    const double max = M_PI * 2;
    value = fmod(value, max);
    if (value < 0)
        value += max;
    return value;
}


bool e172::Math::radiansDirection(double dstAngle, double angle) {
    dstAngle = constrainRadians(dstAngle);
    angle = constrainRadians(angle);
    if ((dstAngle - angle) < 0) {
        if (std::abs(dstAngle - angle) < M_PI) {
            return 1; // //
        } else {
            return 0; //right//
        }
    } else {
        if (std::abs(dstAngle - angle) < M_PI) {
            return 0; //right//
        } else {
            return 1; //left//
        }
    }
}

double e172::Math::radiansDistance(double angle0, double angle1) {
    angle0 = constrainRadians(angle0);
    angle1 = constrainRadians(angle1);

    double max = 2 * M_PI;
    double abs = std::abs(angle0 - angle1);
    return abs > (max * 0.5) ? (max - abs) : abs;
}

double e172::Math::radiansDifference(double angle1, double angle2) {
    double max = 2 * M_PI;
    double diff = angle1 - angle2;
    return std::abs(diff) > (max * 0.5)
            ? ((diff >= 0)
               ? (diff - max)
               : (diff + max)
                 )
            : diff;
}


#ifdef E172_USE_DEGREES_TRANSFORMS

double e172::Math::constrainDegrees(double value) {
    const double max = 360.0;
    value = fmod(value, max);
    if (value < 0)
        value += max;
    return value;
}

bool e172::Math::degreesDirection(double dstAngle, double angle)
{
    if ((dstAngle - angle) < 0) {
        if (std::abs(dstAngle - angle) < 180) {
            return 1; // //
        } else {
            return 0; //right//
        }
    } else {
        if (std::abs(dstAngle - angle) < 180) {
            return 0; //right//
        } else {
            return 1; //left//
        }
    }
}

double e172::Math::degreesDistance(double angle1, double angle2)
{
    angle1 = constrainDegrees(angle1);
    angle2 = constrainDegrees(angle2);

    double max = 360;
    double abs = std::abs(angle1 - angle2);
    return abs > (max * 0.5) ? (max - abs) : abs;
}

double e172::Math::degreesDifference(double angle1, double angle2) {
    double max = 360;
    double diff = angle1 - angle2;
    return std::abs(diff) > (max * 0.5)
            ? ((diff >= 0)
               ? (diff - max)
               : (diff + max)
                 )
            : diff;
}

#endif

double e172::Math::topLimitedFunction(double x) {
    const auto xPlus1 = x + 1;
    if (xPlus1 != null) {
        return 1 - 1 / xPlus1;
    }
    return 0;
}

void e172::Math::concurentInitMatrix(
    size_t w, size_t h, const std::function<void(const std::pair<size_t, size_t> &)> &function)
{
    std::vector<std::pair<size_t, size_t>> job(w * h);
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            job[(y * w) + x] = { x, y };
        }
    }
    std::for_each(std::execution::par_unseq, job.begin(), job.end(), function);
}

float e172::Math::mod(float a, float b)
{
    return fmodf(a, b);
}

double e172::Math::mod(double a, double b)
{
    return fmod(a, b);
}

long double e172::Math::mod(long double a, long double b)
{
    return fmodl(a, b);
}
