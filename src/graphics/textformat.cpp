#include "textformat.h"


namespace e172 {

int TextFormat::fontSize() const {
    return m_fontSize;
}

int TextFormat::fontWidth() const {
    return m_fontSize / 2 + 1;
}

int TextFormat::fontHeight() const {
    return m_fontSize + 1;
}

TextFormat::Alignment TextFormat::alignment() const
{
    return m_alignment;
}

TextFormat::Alignment operator |(TextFormat::Alignment a0, TextFormat::Alignment a1) {
    return static_cast<TextFormat::Alignment>(static_cast<int>(a0) | static_cast<int>(a1));
}

void TextFormat::setAlignment(const Alignment &alignment)
{
    m_alignment = alignment;
}

void TextFormat::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
}

std::string TextFormat::font() const
{
    return m_font;
}

void TextFormat::setFont(const std::string &font)
{
    m_font = font;
}

TextFormat::TextFormat() {
    
}

TextFormat::TextFormat(TextFormat::Alignment alignment) {
    m_alignment = alignment;
}

TextFormat::TextFormat(TextFormat::Alignment alignment, int size) {
    m_alignment = alignment;
    m_fontSize = size;
}

TextFormat TextFormat::fromFont(const std::string &font, int size) {
    TextFormat result = fromFontSize(size);
    result.setFont(font);
    return result;
}

TextFormat TextFormat::fromFontSize(int size) {
    return TextFormat(AlignDefault, size);
}

}
