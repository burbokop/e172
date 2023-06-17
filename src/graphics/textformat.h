// Copyright 2023 Borys Boiko

#pragma once

#include <string>

namespace e172 {

class TextFormat {
public:
    enum Alignment {
        AlignHCenter = 1,
        AlignVCenter = 2,
        AlignLeft = 4,
        AlignRight = 8,
        AlignTop = 16,
        AlignBottom = 32,
        AlignCenter = AlignHCenter | AlignVCenter,
        AlignDefault = AlignRight | AlignBottom
    };

    TextFormat() = default;

    TextFormat(Alignment alignment)
        : m_alignment(alignment)
    {}

    TextFormat(Alignment alignment, int size)
        : m_alignment(alignment)
        , m_fontSize(size)
    {}

    friend Alignment operator|(Alignment a0, Alignment a1);

    static TextFormat fromFont(const std::string &font, int size = 16);
    static TextFormat fromFontSize(int size);

    int fontSize() const { return m_fontSize; }
    int fontWidth() const;
    int fontHeight() const;

    Alignment alignment() const { return m_alignment; }
    void setAlignment(const Alignment &alignment) { m_alignment = alignment; }
    void setFontSize(int fontSize) { m_fontSize = fontSize; }
    const std::string &font() const { return m_font; }
    void setFont(const std::string &font) { m_font = font; }

private:
    Alignment m_alignment = AlignRight | AlignBottom;
    int m_fontSize = 0;
    std::string m_font;
};

} // namespace e172
