#include "differentiator.h"
#include "math.h"

e172::Differentiator::Differentiator(Differentiator::XMode xMode) {
    m_xMode = xMode;
}

double e172::Differentiator::proceed(double value) {
    if(last != value) {
        if(m_xMode == Time) {
            const auto dt = c.deltaTime();
            if(hasLast && dt != e172::Math::null) {
                derivative = (value - hasLast) / dt;
            }
            c.update();
        } else {
            if(hasLast) {
                derivative = value - hasLast;
            }
        }
    }

    last = value;
    hasLast = true;
    return derivative;
}

void e172::Differentiator::reset() {
    hasLast = false;
    derivative = 0;
}
