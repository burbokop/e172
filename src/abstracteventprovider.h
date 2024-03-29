// Copyright 2023 Borys Boiko

#pragma once

#include "math/vector.h"
#include "net/common.h"
#include "utility/buffer.h"
#include <optional>
#include <queue>

namespace e172 {

/**
 * @brief Scancode - keyboard codes
 * 
 */
enum Scancode : std::uint16_t {
    ScancodeUnknown = 0,

    /**
     * @name Usage page 0x07
     * Values below are from usage page 0x07 (USB keyboard page).
     */

    ScancodeA = 4,
    ScancodeB = 5,
    ScancodeC = 6,
    ScancodeD = 7,
    ScancodeE = 8,
    ScancodeF = 9,
    ScancodeG = 10,
    ScancodeH = 11,
    ScancodeI = 12,
    ScancodeJ = 13,
    ScancodeK = 14,
    ScancodeL = 15,
    ScancodeM = 16,
    ScancodeN = 17,
    ScancodeO = 18,
    ScancodeP = 19,
    ScancodeQ = 20,
    ScancodeR = 21,
    ScancodeS = 22,
    ScancodeT = 23,
    ScancodeU = 24,
    ScancodeV = 25,
    ScancodeW = 26,
    ScancodeX = 27,
    ScancodeY = 28,
    ScancodeZ = 29,

    Scancode1 = 30,
    Scancode2 = 31,
    Scancode3 = 32,
    Scancode4 = 33,
    Scancode5 = 34,
    Scancode6 = 35,
    Scancode7 = 36,
    Scancode8 = 37,
    Scancode9 = 38,
    Scancode0 = 39,

    ScancodeReturn = 40,
    ScancodeEscape = 41,
    ScancodeBackSpace = 42,
    ScancodeTab = 43,
    ScancodeSpace = 44,

    ScancodeMinus = 45,
    ScancodeEquals = 46,
    ScancodeLeftBracket = 47,
    ScancodeRightBracket = 48,

    /** 
     * Located at the lower left of the return
     * key on ISO keyboards and at the right end
     * of the QWERTY row on ANSI keyboards.
     * Produces REVERSE SOLIDUS (backslash) and
     * VERTICAL LINE in a US layout, REVERSE
     * SOLIDUS and VERTICAL LINE in a UK Mac
     * layout, NUMBER SIGN and TILDE in a UK
     * Windows layout, DOLLAR SIGN and POUND SIGN
     * in a Swiss German layout, NUMBER SIGN and
     * APOSTROPHE in a German layout, GRAVE
     * ACCENT and POUND SIGN in a French Mac
     * layout, and ASTERISK and MICRO SIGN in a
     * French Windows layout.
     */
    ScancodeBackslash = 49,

    /** 
     * ISO USB keyboards actually use this code
     * instead of 49 for the same key, but all
     * OSes I've seen treat the two codes
     * identically. So, as an implementor, unless
     * your keyboard generates both of those
     * codes and your OS treats them differently,
     * you should generate ScancodeBackslash
     * instead of this code. As a user, you
     * should not rely on this code because SDL
     * will never generate it with most (all?)
     * keyboards.
     */
    ScancodeNonUSHash = 50,
    ScancodeSemicolon = 51,
    ScancodeApostrophe = 52,

    /**
     * Located in the top left corner (on both ANSI
     * and ISO keyboards). Produces GRAVE ACCENT and
     * TILDE in a US Windows layout and in US and UK
     * Mac layouts on ANSI keyboards, GRAVE ACCENT
     * and NOT SIGN in a UK Windows layout, SECTION
     * SIGN and PLUS-MINUS SIGN in US and UK Mac
     * layouts on ISO keyboards, SECTION SIGN and
     * DEGREE SIGN in a Swiss German layout (Mac:
     * only on ISO keyboards), CIRCUMFLEX ACCENT and
     * DEGREE SIGN in a German layout (Mac: only on
     * ISO keyboards), SUPERSCRIPT TWO and TILDE in a
     * French Windows layout, COMMERCIAL AT and
     * NUMBER SIGN in a French Mac layout on ISO
     * keyboards, and LESS-THAN SIGN and GREATER-THAN
     * SIGN in a Swiss German, German, or French Mac
     * layout on ANSI keyboards.
     */
    ScancodeGrave = 53,
    ScancodeComma = 54,
    ScancodePeriod = 55,
    ScancodeSlash = 56,

    ScancodeCapsLock = 57,

    ScancodeF1 = 58,
    ScancodeF2 = 59,
    ScancodeF3 = 60,
    ScancodeF4 = 61,
    ScancodeF5 = 62,
    ScancodeF6 = 63,
    ScancodeF7 = 64,
    ScancodeF8 = 65,
    ScancodeF9 = 66,
    ScancodeF10 = 67,
    ScancodeF11 = 68,
    ScancodeF12 = 69,

    ScancodePrintScreen = 70,
    ScancodeScrollLock = 71,
    ScancodePause = 72,

    /**
     * insert on PC, help on some Mac keyboards (but
     * does send code 73, not 117)
     */
    ScancodeInsert = 73,
    ScancodeHome = 74,
    ScancodePageUp = 75,
    ScancodeDelete = 76,
    ScancodeEnd = 77,
    ScancodePageDown = 78,
    ScancodeRight = 79,
    ScancodeLeft = 80,
    ScancodeDown = 81,
    ScancodeUp = 82,

    /**
     * num lock on PC, clear on Mac keyboards
     */
    ScancodeNumLockClear = 83,
    ScancodeKpDivide = 84,
    ScancodeKpMultiply = 85,
    ScancodeKpMinus = 86,
    ScancodeKpPlus = 87,
    ScancodeKpEnter = 88,
    ScancodeKp1 = 89,
    ScancodeKp2 = 90,
    ScancodeKp3 = 91,
    ScancodeKp4 = 92,
    ScancodeKp5 = 93,
    ScancodeKp6 = 94,
    ScancodeKp7 = 95,
    ScancodeKp8 = 96,
    ScancodeKp9 = 97,
    ScancodeKp0 = 98,
    ScancodeKpPeriod = 99,

    /** 
     * This is the additional key that ISO
     * keyboards have over ANSI ones,
     * located between left shift and Y.
     * Produces GRAVE ACCENT and TILDE in a
     * US or UK Mac layout, REVERSE SOLIDUS
     * (backslash) and VERTICAL LINE in a
     * US or UK Windows layout, and
     * LESS-THAN SIGN and GREATER-THAN SIGN
     * in a Swiss German, German, or French
     * layout.
     */
    ScancodeNonUSBackSlash = 100,

    /**
     * windows contextual menu, compose
     */
    ScancodeApplication = 101,

    /**
     * The USB document says this is a status flag,
     * not a physical key - but some Mac keyboards
     * do have a power key.
     */
    ScancodePower = 102,
    ScancodeKPEquals = 103,
    ScancodeF13 = 104,
    ScancodeF14 = 105,
    ScancodeF15 = 106,
    ScancodeF16 = 107,
    ScancodeF17 = 108,
    ScancodeF18 = 109,
    ScancodeF19 = 110,
    ScancodeF20 = 111,
    ScancodeF21 = 112,
    ScancodeF22 = 113,
    ScancodeF23 = 114,
    ScancodeF24 = 115,
    ScancodeExecute = 116,
    ScancodeHelp = 117,
    ScancodeMenu = 118,
    ScancodeSelect = 119,
    ScancodeStop = 120,

    /**
     * redo
     */
    ScancodeAgain = 121,
    ScancodeUndo = 122,
    ScancodeCut = 123,
    ScancodeCopy = 124,
    ScancodePaste = 125,
    ScancodeFind = 126,
    ScancodeMute = 127,
    ScancodeVolumeUp = 128,
    ScancodeVolumeDown = 129,
    /* not sure whether there's a reason to enable these */
    /*     ScancodeLOCKINGCAPSLOCK = 130,                */
    /*     ScancodeLOCKINGNUMLOCK = 131,                 */
    /*     ScancodeLOCKINGSCROLLLOCK = 132,              */
    ScancodeKPComma = 133,
    ScancodeKPEqualSas400 = 134,

    /**
     * used on Asian keyboards, see
     * footnotes in USB doc
     */
    ScancodeInternational1 = 135,
    ScancodeInternational2 = 136,

    /**
     * Yen
     */
    ScancodeInternational3 = 137,
    ScancodeInternational4 = 138,
    ScancodeInternational5 = 139,
    ScancodeInternational6 = 140,
    ScancodeInternational7 = 141,
    ScancodeInternational8 = 142,
    ScancodeInternational9 = 143,

    /**
     * Hangul/English toggle
     */
    ScancodeLang1 = 144,

    /**
     * Hanja conversion
     */
    ScancodeLang2 = 145,

    /**
     * Katakana
     */
    ScancodeLang3 = 146,

    /**
     * Hiragana
     */
    ScancodeLang4 = 147,

    /**
     * Zenkaku/Hankaku
     */
    ScancodeLang5 = 148,

    /**
     * reserved
     */
    ScancodeLang6 = 149,

    /**
     * reserved
     */
    ScancodeLang7 = 150,

    /**
     * reserved
     */
    ScancodeLang8 = 151,

    /**
     * reserved
     */
    ScancodeLang9 = 152,

    /**
     * Erase-Eaze
     */
    ScancodeAltErase = 153,
    ScancodeSysReq = 154,
    ScancodeCancel = 155,
    ScancodeClear = 156,
    ScancodePrior = 157,
    ScancodeReturn2 = 158,
    ScancodeSeparator = 159,
    ScancodeOut = 160,
    ScancodeOper = 161,
    ScancodeClearAgain = 162,
    ScancodeCrsel = 163,
    ScancodeExsel = 164,

    ScancodeKP00 = 176,
    ScancodeKP000 = 177,
    ScancodeThousandsSeparator = 178,
    ScancodeDecimalSeparator = 179,
    ScancodeCurrencyUnit = 180,
    ScancodeCurrencySubunit = 181,
    ScancodeKpLeftParen = 182,
    ScancodeKpRightParen = 183,
    ScancodeKpLeftBrace = 184,
    ScancodeKpRightBrace = 185,
    ScancodeKpTab = 186,
    ScancodeKpBackspace = 187,
    ScancodeKpA = 188,
    ScancodeKpB = 189,
    ScancodeKpC = 190,
    ScancodeKpD = 191,
    ScancodeKpE = 192,
    ScancodeKpF = 193,
    ScancodeKpXor = 194,
    ScancodeKpPower = 195,
    ScancodeKpPercent = 196,
    ScancodeKpLess = 197,
    ScancodeKpGreater = 198,
    ScancodeKpAmpersand = 199,
    ScancodeKpDblAmpersand = 200,
    ScancodeKpVerticalBar = 201,
    ScancodeKpDblVerticalBar = 202,
    ScancodeKpColon = 203,
    ScancodeKpHash = 204,
    ScancodeKpSpace = 205,
    ScancodeKpAt = 206,
    ScancodeKpExclam = 207,
    ScancodeKpMemStore = 208,
    ScancodeKpMemRecall = 209,
    ScancodeKpMemClear = 210,
    ScancodeKpMemAdd = 211,
    ScancodeKpMemSubtract = 212,
    ScancodeKpMemMultiply = 213,
    ScancodeKpMemDivide = 214,
    ScancodeKpPlusMinus = 215,
    ScancodeKpClear = 216,
    ScancodeKpClearEntry = 217,
    ScancodeKpBinary = 218,
    ScancodeKpOctal = 219,
    ScancodeKpDecimal = 220,
    ScancodeKpHexadecimal = 221,

    ScancodeLCtrl = 224,
    ScancodeLShift = 225,

    /**
     * alt, option
     */
    ScancodeLAlt = 226,

    /**
     * windows, command (apple), meta
     */
    ScancodeLGui = 227,
    ScancodeRCtrl = 228,
    ScancodeRShift = 229,

    /**   
     * alt gr, option
     */
    ScancodeRAlt = 230,

    /**
     * windows, command (apple), meta
     */
    ScancodeRGui = 231,

    /**
     * I'm not sure if this is really not covered
     * by any of the above, but since there's a
     * special KMOD_MODE for it I'm adding it here
     */
    ScancodeMode = 257,

    /**
     * @name Usage page 0x0C
     * Values below are mapped from usage page 0x0C (USB consumer page).
     */

    ScancodeAudioNext = 258,
    ScancodeAudioPrev = 259,
    ScancodeAudioStop = 260,
    ScancodeAudioPlay = 261,
    ScancodeAudioMute = 262,
    ScancodeMediaSelect = 263,
    ScancodeWWW = 264,
    ScancodeMail = 265,
    ScancodeCalculator = 266,
    ScancodeComputer = 267,
    ScancodeAcSearch = 268,
    ScancodeAcHome = 269,
    ScancodeAcBack = 270,
    ScancodeAcForward = 271,
    ScancodeAcStop = 272,
    ScancodeAcRefresh = 273,
    ScancodeAcBookmarks = 274,

    /**
     * @name Walther keys
     * Values below that Christian Walther added (for mac keyboard?).
     */

    ScancodeBrightnessDown = 275,
    ScancodeBrightnessUp = 276,

    /**
     * display mirroring/dual display
     * switch, video mode switch
     */
    ScancodeDisplaySwitch = 277,
    ScancodeKbDillumToggle = 278,
    ScancodeKbDillumDown = 279,
    ScancodeKbDillumUp = 280,
    ScancodeEject = 281,
    ScancodeSleep = 282,

    ScancodeApp1 = 283,
    ScancodeApp2 = 284,

    /**
     * @name Usage page 0x0C (additional media keys)
     * Values below are mapped from usage page 0x0C (USB consumer page).
     */

    ScancodeAudioRewind = 285,
    ScancodeAudioFastForward = 286,

    /**
     * @name Mobile keys
     * values below that are often used on mobile phones.
     */

    /** 
     * Usually situated below the display on phones and
     * used as a multi-function feature key for selecting
     * a software defined function shown on the bottom left
     * of the display.
     */
    ScancodeSoftLeft = 287,

    /**
     * Usually situated below the display on phones and
     * used as a multi-function feature key for selecting
     * a software defined function shown on the bottom right
     * of the display.
     */
    ScancodeSoftRight = 288,

    /**
     * Used for accepting phone calls.
     */
    ScancodeCall = 289,

    /**
     * Used for rejecting phone calls.
     */
    ScancodeEndCall = 290,

    /**
     * Values below are utility
     */

    /**
     * not a key, just marks the number of scancodes
     * for array bounds
     */
    ScancodesCount = 512
};

std::ostream &operator<<(std::ostream &stream, Scancode scancode);

class Event
{
public:
    enum Type : std::uint8_t { KeyDown, KeyUp, MouseMotion, Quit };
    using Pos = Vector<std::uint16_t>;

    Type type() const { return m_type; };

    std::optional<Scancode> scancode() const
    {
        return Data::hasScancode(m_type) ? std::optional<Scancode>(m_data.scancode) : std::nullopt;
    };

    std::optional<Pos> pos() const
    {
        return Data::hasPos(m_type) ? std::optional<Pos>(m_data.pos) : std::nullopt;
    };

    std::optional<PackedClientId> clientId() const { return m_clientId; }

    void serialize(WriteBuffer &buf) const;
    static std::optional<Event> deserialize(ReadBuffer &buf);
    static std::optional<Event> deserializeConsume(ReadBuffer &&buf) { return deserialize(buf); }

    static Event keyDown(Scancode scancode) { return Event(std::nullopt, KeyDown, Data(scancode)); }

    static Event keyUp(Scancode scancode) { return Event(std::nullopt, KeyUp, Data(scancode)); }

    static Event mouseMotion(const Pos &pos) { return Event(std::nullopt, MouseMotion, Data(pos)); }

    static Event quit() { return Event(std::nullopt, Quit, ScancodeUnknown); }

    Event claimClientId(PackedClientId clientId) const { return Event(clientId, m_type, m_data); }

    inline friend std::ostream &operator<<(std::ostream &stream, const Event &event)
    {
        switch (event.m_type) {
        case Event::KeyDown:
            return stream << "KeyDown { " << event.scancode().value() << " }";
        case Event::KeyUp:
            return stream << "KeyUp { " << event.scancode().value() << " }";
        case Event::MouseMotion:
            return stream << "MouseMotion { " << event.pos().value() << " }";
        case Event::Quit:
            return stream << "Quit";
        }
        return stream;
    }

private:
    Event withClientId(const std::optional<PackedClientId> &clientId) const
    {
        return Event(clientId, m_type, m_data);
    }

    union Data {
        Data(Scancode s)
            : scancode(s)
        {}

        Data(Pos s)
            : pos(s)
        {}

        static bool hasScancode(Event::Type t) { return t == KeyDown || t == KeyUp; }
        static bool hasPos(Event::Type t) { return t == MouseMotion; }

        Scancode scancode;
        Pos pos;
    };

    explicit Event(const std::optional<PackedClientId> &clientId, Type type, Data data)
        : m_clientId(clientId)
        , m_type(type)
        , m_data(data)
    {}

private:
    std::optional<PackedClientId> m_clientId;
    Type m_type;
    Data m_data;
};

class AbstractEventProvider
{
public:
    AbstractEventProvider() = default;

    virtual std::optional<Event> pullEvent() = 0;

    virtual ~AbstractEventProvider() = default;
};

/**
 * @brief The MemEventProvider class provides events from application memory
 * Can be used as stub in tests or proxy to be connected to another event system
 */
class MemEventProvider : public AbstractEventProvider
{
public:
    MemEventProvider() = default;
    void pushEvent(const Event &event) { m_queue.push(event); }

    // AbstractEventProvider interface
public:
    std::optional<Event> pullEvent() override
    {
        if (m_queue.empty()) {
            return std::nullopt;
        } else {
            const auto e = m_queue.front();
            m_queue.pop();
            return e;
        }
    }

private:
    std::queue<Event> m_queue;
};

} // namespace e172
