// Copyright 2023 Borys Boiko

#pragma once

#include <cstdint>

namespace e172 {

using Color = std::uint32_t;
using ColorComponent = std::uint8_t;

/**
 * @brief argb
 * @param alpha
 * @param r
 * @param g
 * @param b
 * @return color
 * @note all arguments from 0 to 255
 */
constexpr Color argb(ColorComponent alpha, ColorComponent r, ColorComponent g, ColorComponent b)
{
    return (static_cast<Color>(alpha) << 24) | (static_cast<Color>(r) << 16)
           | (static_cast<Color>(g) << 8) | static_cast<Color>(b);
}

/**
 * @brief rgb
 * @param r
 * @param g
 * @param b
 * @return
 * @note all arguments from 0 to 255
 */
constexpr Color rgb(ColorComponent r, ColorComponent g, ColorComponent b)
{
    return argb(0xff, r, g, b);
}

constexpr ColorComponent alpha(Color color)
{
    return color >> 24;
}

constexpr ColorComponent red(Color color)
{
    return color >> 16;
}

constexpr ColorComponent green(Color color)
{
    return color >> 8;
}

constexpr ColorComponent blue(Color color)
{
    return color >> 0;
}

class Random;

Color randomColor(Random &random);
Color randomColor(Random &&random);

Color blend(Color top, Color bottom);

} // namespace e172
