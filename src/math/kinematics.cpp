// Copyright 2023 Borys Boiko

#include "kinematics.h"

#include "math.h"
#include <cmath>

namespace e172 {

double eFunction(double x, double c)
{
    if (c != Math::null) {
        const auto y2 = 1 - std::pow(x, 2) / std::pow(c, 2);
        if (y2 >= 0) {
            return std::sqrt(y2);
        }
    }
    return 0;
}

} // namespace e172
