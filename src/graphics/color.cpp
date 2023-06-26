// Copyright 2023 Borys Boiko

#include "color.h"

#include "../utility/random.h"
#include <cmath>

namespace e172 {

Color randomColor(Random &random)
{
    return random.next<Color>() % static_cast<Color>(std::pow(2, 24));
}

Color randomColor(Random &&random)
{
    return random.next<Color>() % static_cast<Color>(std::pow(2, 24));
}

Color blendPixels(Color top, Color bottom)
{
    const auto bA = uint8_t((top >> 0) & 0x000000ff);
    const auto gA = uint8_t((top >> 8) & 0x000000ff);
    const auto rA = uint8_t((top >> 16) & 0x000000ff);
    const auto aA = uint8_t((top >> 24) & 0x000000ff);

    const auto bB = uint8_t((bottom >> 0) & 0x000000ff);
    const auto gB = uint8_t((bottom >> 8) & 0x000000ff);
    const auto rB = uint8_t((bottom >> 16) & 0x000000ff);
    const auto aB = uint8_t((bottom >> 24) & 0x000000ff);

    const uint32_t bOut = uint32_t((bA * aA / 255) + (bB * aB * (255 - aA) / (255 * 255))) << 0;
    const uint32_t gOut = uint32_t((gA * aA / 255) + (gB * aB * (255 - aA) / (255 * 255))) << 8;
    const uint32_t rOut = uint32_t((rA * aA / 255) + (rB * aB * (255 - aA) / (255 * 255))) << 16;
    const uint32_t aOut = uint32_t(aA + (aB * (255 - aA) / 255)) << 24;

    return aOut | rOut | gOut | bOut;
}

} // namespace e172
