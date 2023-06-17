// Copyright 2023 Borys Boiko

#include "textformat.h"

namespace e172 {

int TextFormat::fontWidth() const
{
    return m_fontSize / 2 + 1;
}

int TextFormat::fontHeight() const {
    return m_fontSize + 1;
}

TextFormat::Alignment operator |(TextFormat::Alignment a0, TextFormat::Alignment a1) {
    return static_cast<TextFormat::Alignment>(static_cast<int>(a0) | static_cast<int>(a1));
}

TextFormat TextFormat::fromFont(const std::string &font, int size) {
    TextFormat result = fromFontSize(size);
    result.setFont(font);
    return result;
}

TextFormat TextFormat::fromFontSize(int size) {
    return TextFormat(AlignDefault, size);
}

} // namespace e172
