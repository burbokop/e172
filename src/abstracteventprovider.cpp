// Copyright 2023 Borys Boiko

#include "abstracteventprovider.h"

namespace e172 {

void Event::serialize(WriteBuffer &buf) const
{
    buf.writeWithLoss(m_clientId);
    buf.write(m_type);
    switch (m_type) {
    case Event::KeyDown:
        buf.write(m_data.scancode);
        break;
    case Event::KeyUp:
        buf.write(m_data.scancode);
        break;
    case Event::MouseMotion:
        buf.write(m_data.pos);
        break;
    case Event::Quit:
        break;
    }
}

std::optional<Event> Event::deserialize(ReadBuffer &buf)
{
    const auto clientId = buf.readWithLoss<std::optional<PackedClientId>>();
    if (!clientId)
        return std::nullopt;

    const auto type = buf.read<Event::Type>();
    if (!type)
        return std::nullopt;

    switch (*type) {
    case KeyDown:
        if (const auto &s = buf.read<Scancode>()) {
            return Event::keyDown(*s).withClientId(*clientId);
        } else {
            return std::nullopt;
        }
    case KeyUp:
        if (const auto &s = buf.read<Scancode>()) {
            return Event::keyUp(*s).withClientId(*clientId);
        } else {
            return std::nullopt;
        }
    case MouseMotion:
        if (const auto &s = buf.read<Pos>()) {
            return Event::mouseMotion(*s).withClientId(*clientId);
        } else {
            return std::nullopt;
        }
    case Quit:
        return Event::quit().withClientId(*clientId);
    }
    throw std::runtime_error("Event::deserialize: Unknown type");
}

std::ostream &operator<<(std::ostream &stream, Scancode scancode)
{
    switch (scancode) {
    case ScancodeUnknown:
        return stream << "Unknown";
    case ScancodeA:
        return stream << "A";
    case ScancodeB:
        return stream << "B";
    case ScancodeC:
        return stream << "C";
    case ScancodeD:
        return stream << "D";
    case ScancodeE:
        return stream << "E";
    case ScancodeF:
        return stream << "F";
    case ScancodeG:
        return stream << "G";
    case ScancodeH:
        return stream << "H";
    case ScancodeI:
        return stream << "I";
    case ScancodeJ:
        return stream << "J";
    case ScancodeK:
        return stream << "K";
    case ScancodeL:
        return stream << "L";
    case ScancodeM:
        return stream << "M";
    case ScancodeN:
        return stream << "N";
    case ScancodeO:
        return stream << "O";
    case ScancodeP:
        return stream << "P";
    case ScancodeQ:
        return stream << "Q";
    case ScancodeR:
        return stream << "R";
    case ScancodeS:
        return stream << "S";
    case ScancodeT:
        return stream << "T";
    case ScancodeU:
        return stream << "U";
    case ScancodeV:
        return stream << "V";
    case ScancodeW:
        return stream << "W";
    case ScancodeX:
        return stream << "X";
    case ScancodeY:
        return stream << "Y";
    case ScancodeZ:
        return stream << "Z";
    case Scancode1:
        return stream << "1";
    case Scancode2:
        return stream << "2";
    case Scancode3:
        return stream << "3";
    case Scancode4:
        return stream << "4";
    case Scancode5:
        return stream << "5";
    case Scancode6:
        return stream << "6";
    case Scancode7:
        return stream << "7";
    case Scancode8:
        return stream << "8";
    case Scancode9:
        return stream << "9";
    case Scancode0:
        return stream << "0";
    case ScancodeReturn:
        return stream << "Return";
    case ScancodeEscape:
        return stream << "Escape";
    case ScancodeBackSpace:
        return stream << "BackSpace";
    case ScancodeTab:
        return stream << "Tab";
    case ScancodeSpace:
        return stream << "Space";
    case ScancodeMinus:
        return stream << "Minus";
    case ScancodeEquals:
        return stream << "Equals";
    case ScancodeLeftBracket:
        return stream << "LeftBracket";
    case ScancodeRightBracket:
        return stream << "RightBracket";
    case ScancodeBackslash:
        return stream << "Backslash";
    case ScancodeNonUSHash:
        return stream << "NonUSHash";
    case ScancodeSemicolon:
        return stream << "Semicolon";
    case ScancodeApostrophe:
        return stream << "Apostrophe";
    case ScancodeGrave:
        return stream << "Grave";
    case ScancodeComma:
        return stream << "Comma";
    case ScancodePeriod:
        return stream << "Period";
    case ScancodeSlash:
        return stream << "Slash";
    case ScancodeCapsLock:
        return stream << "CapsLock";
    case ScancodeF1:
        return stream << "F1";
    case ScancodeF2:
        return stream << "F2";
    case ScancodeF3:
        return stream << "F3";
    case ScancodeF4:
        return stream << "F4";
    case ScancodeF5:
        return stream << "F5";
    case ScancodeF6:
        return stream << "F6";
    case ScancodeF7:
        return stream << "F7";
    case ScancodeF8:
        return stream << "F8";
    case ScancodeF9:
        return stream << "F9";
    case ScancodeF10:
        return stream << "F10";
    case ScancodeF11:
        return stream << "F11";
    case ScancodeF12:
        return stream << "F12";
    case ScancodePrintScreen:
        return stream << "PrintScreen";
    case ScancodeScrollLock:
        return stream << "ScrollLock";
    case ScancodePause:
        return stream << "Pause";
    case ScancodeInsert:
        return stream << "Insert";
    case ScancodeHome:
        return stream << "Home";
    case ScancodePageUp:
        return stream << "PageUp";
    case ScancodeDelete:
        return stream << "Delete";
    case ScancodeEnd:
        return stream << "End";
    case ScancodePageDown:
        return stream << "PageDown";
    case ScancodeRight:
        return stream << "Right";
    case ScancodeLeft:
        return stream << "Left";
    case ScancodeDown:
        return stream << "Down";
    case ScancodeUp:
        return stream << "Up";
    case ScancodeNumLockClear:
        return stream << "NumLockClear";
    case ScancodeKpDivide:
        return stream << "KpDivide";
    case ScancodeKpMultiply:
        return stream << "KpMultiply";
    case ScancodeKpMinus:
        return stream << "KpMinus";
    case ScancodeKpPlus:
        return stream << "KpPlus";
    case ScancodeKpEnter:
        return stream << "KpEnter";
    case ScancodeKp1:
        return stream << "Kp1";
    case ScancodeKp2:
        return stream << "Kp2";
    case ScancodeKp3:
        return stream << "Kp3";
    case ScancodeKp4:
        return stream << "Kp4";
    case ScancodeKp5:
        return stream << "Kp5";
    case ScancodeKp6:
        return stream << "Kp6";
    case ScancodeKp7:
        return stream << "Kp7";
    case ScancodeKp8:
        return stream << "Kp8";
    case ScancodeKp9:
        return stream << "Kp9";
    case ScancodeKp0:
        return stream << "Kp0";
    case ScancodeKpPeriod:
        return stream << "KpPeriod";
    case ScancodeNonUSBackSlash:
        return stream << "NonUSBackSlash";
    case ScancodeApplication:
        return stream << "Application";
    case ScancodePower:
        return stream << "Power";
    case ScancodeKPEquals:
        return stream << "KPEquals";
    case ScancodeF13:
        return stream << "F13";
    case ScancodeF14:
        return stream << "F14";
    case ScancodeF15:
        return stream << "F15";
    case ScancodeF16:
        return stream << "F16";
    case ScancodeF17:
        return stream << "F17";
    case ScancodeF18:
        return stream << "F18";
    case ScancodeF19:
        return stream << "F19";
    case ScancodeF20:
        return stream << "F20";
    case ScancodeF21:
        return stream << "F21";
    case ScancodeF22:
        return stream << "F22";
    case ScancodeF23:
        return stream << "F23";
    case ScancodeF24:
        return stream << "F24";
    case ScancodeExecute:
        return stream << "Execute";
    case ScancodeHelp:
        return stream << "Help";
    case ScancodeMenu:
        return stream << "Menu";
    case ScancodeSelect:
        return stream << "Select";
    case ScancodeStop:
        return stream << "Stop";
    case ScancodeAgain:
        return stream << "Again";
    case ScancodeUndo:
        return stream << "Undo";
    case ScancodeCut:
        return stream << "Cut";
    case ScancodeCopy:
        return stream << "Copy";
    case ScancodePaste:
        return stream << "Paste";
    case ScancodeFind:
        return stream << "Find";
    case ScancodeMute:
        return stream << "Mute";
    case ScancodeVolumeUp:
        return stream << "VolumeUp";
    case ScancodeVolumeDown:
        return stream << "VolumeDown";
    case ScancodeKPComma:
        return stream << "KPComma";
    case ScancodeKPEqualSas400:
        return stream << "KPEqualSas400";
    case ScancodeInternational1:
        return stream << "International1";
    case ScancodeInternational2:
        return stream << "International2";
    case ScancodeInternational3:
        return stream << "International3";
    case ScancodeInternational4:
        return stream << "International4";
    case ScancodeInternational5:
        return stream << "International5";
    case ScancodeInternational6:
        return stream << "International6";
    case ScancodeInternational7:
        return stream << "International7";
    case ScancodeInternational8:
        return stream << "International8";
    case ScancodeInternational9:
        return stream << "International9";
    case ScancodeLang1:
        return stream << "Lang1";
    case ScancodeLang2:
        return stream << "Lang2";
    case ScancodeLang3:
        return stream << "Lang3";
    case ScancodeLang4:
        return stream << "Lang4";
    case ScancodeLang5:
        return stream << "Lang5";
    case ScancodeLang6:
        return stream << "Lang6";
    case ScancodeLang7:
        return stream << "Lang7";
    case ScancodeLang8:
        return stream << "Lang8";
    case ScancodeLang9:
        return stream << "Lang9";
    case ScancodeAltErase:
        return stream << "AltErase";
    case ScancodeSysReq:
        return stream << "SysReq";
    case ScancodeCancel:
        return stream << "Cancel";
    case ScancodeClear:
        return stream << "Clear";
    case ScancodePrior:
        return stream << "Prior";
    case ScancodeReturn2:
        return stream << "Return2";
    case ScancodeSeparator:
        return stream << "Separator";
    case ScancodeOut:
        return stream << "Out";
    case ScancodeOper:
        return stream << "Oper";
    case ScancodeClearAgain:
        return stream << "ClearAgain";
    case ScancodeCrsel:
        return stream << "Crsel";
    case ScancodeExsel:
        return stream << "Exsel";
    case ScancodeKP00:
        return stream << "KP00";
    case ScancodeKP000:
        return stream << "KP000";
    case ScancodeThousandsSeparator:
        return stream << "ThousandsSeparator";
    case ScancodeDecimalSeparator:
        return stream << "DecimalSeparator";
    case ScancodeCurrencyUnit:
        return stream << "CurrencyUnit";
    case ScancodeCurrencySubunit:
        return stream << "CurrencySubunit";
    case ScancodeKpLeftParen:
        return stream << "KpLeftParen";
    case ScancodeKpRightParen:
        return stream << "KpRightParen";
    case ScancodeKpLeftBrace:
        return stream << "KpLeftBrace";
    case ScancodeKpRightBrace:
        return stream << "KpRightBrace";
    case ScancodeKpTab:
        return stream << "KpTab";
    case ScancodeKpBackspace:
        return stream << "KpBackspace";
    case ScancodeKpA:
        return stream << "KpA";
    case ScancodeKpB:
        return stream << "KpB";
    case ScancodeKpC:
        return stream << "KpC";
    case ScancodeKpD:
        return stream << "KpD";
    case ScancodeKpE:
        return stream << "KpE";
    case ScancodeKpF:
        return stream << "KpF";
    case ScancodeKpXor:
        return stream << "KpXor";
    case ScancodeKpPower:
        return stream << "KpPower";
    case ScancodeKpPercent:
        return stream << "KpPercent";
    case ScancodeKpLess:
        return stream << "KpLess";
    case ScancodeKpGreater:
        return stream << "KpGreater";
    case ScancodeKpAmpersand:
        return stream << "KpAmpersand";
    case ScancodeKpDblAmpersand:
        return stream << "KpDblAmpersand";
    case ScancodeKpVerticalBar:
        return stream << "KpVerticalBar";
    case ScancodeKpDblVerticalBar:
        return stream << "KpDblVerticalBar";
    case ScancodeKpColon:
        return stream << "KpColon";
    case ScancodeKpHash:
        return stream << "KpHash";
    case ScancodeKpSpace:
        return stream << "KpSpace";
    case ScancodeKpAt:
        return stream << "KpAt";
    case ScancodeKpExclam:
        return stream << "KpExclam";
    case ScancodeKpMemStore:
        return stream << "KpMemStore";
    case ScancodeKpMemRecall:
        return stream << "KpMemRecall";
    case ScancodeKpMemClear:
        return stream << "KpMemClear";
    case ScancodeKpMemAdd:
        return stream << "KpMemAdd";
    case ScancodeKpMemSubtract:
        return stream << "KpMemSubtract";
    case ScancodeKpMemMultiply:
        return stream << "KpMemMultiply";
    case ScancodeKpMemDivide:
        return stream << "KpMemDivide";
    case ScancodeKpPlusMinus:
        return stream << "KpPlusMinus";
    case ScancodeKpClear:
        return stream << "KpClear";
    case ScancodeKpClearEntry:
        return stream << "KpClearEntry";
    case ScancodeKpBinary:
        return stream << "KpBinary";
    case ScancodeKpOctal:
        return stream << "KpOctal";
    case ScancodeKpDecimal:
        return stream << "KpDecimal";
    case ScancodeKpHexadecimal:
        return stream << "KpHexadecimal";
    case ScancodeLCtrl:
        return stream << "LCtrl";
    case ScancodeLShift:
        return stream << "LShift";
    case ScancodeLAlt:
        return stream << "LAlt";
    case ScancodeLGui:
        return stream << "LGui";
    case ScancodeRCtrl:
        return stream << "RCtrl";
    case ScancodeRShift:
        return stream << "RShift";
    case ScancodeRAlt:
        return stream << "RAlt";
    case ScancodeRGui:
        return stream << "RGui";
    case ScancodeMode:
        return stream << "Mode";
    case ScancodeAudioNext:
        return stream << "AudioNext";
    case ScancodeAudioPrev:
        return stream << "AudioPrev";
    case ScancodeAudioStop:
        return stream << "AudioStop";
    case ScancodeAudioPlay:
        return stream << "AudioPlay";
    case ScancodeAudioMute:
        return stream << "AudioMute";
    case ScancodeMediaSelect:
        return stream << "MediaSelect";
    case ScancodeWWW:
        return stream << "WWW";
    case ScancodeMail:
        return stream << "Mail";
    case ScancodeCalculator:
        return stream << "Calculator";
    case ScancodeComputer:
        return stream << "Computer";
    case ScancodeAcSearch:
        return stream << "AcSearch";
    case ScancodeAcHome:
        return stream << "AcHome";
    case ScancodeAcBack:
        return stream << "AcBack";
    case ScancodeAcForward:
        return stream << "AcForward";
    case ScancodeAcStop:
        return stream << "AcStop";
    case ScancodeAcRefresh:
        return stream << "AcRefresh";
    case ScancodeAcBookmarks:
        return stream << "AcBookmarks";
    case ScancodeBrightnessDown:
        return stream << "BrightnessDown";
    case ScancodeBrightnessUp:
        return stream << "BrightnessUp";
    case ScancodeDisplaySwitch:
        return stream << "DisplaySwitch";
    case ScancodeKbDillumToggle:
        return stream << "KbDillumToggle";
    case ScancodeKbDillumDown:
        return stream << "KbDillumDown";
    case ScancodeKbDillumUp:
        return stream << "KbDillumUp";
    case ScancodeEject:
        return stream << "Eject";
    case ScancodeSleep:
        return stream << "Sleep";
    case ScancodeApp1:
        return stream << "App1";
    case ScancodeApp2:
        return stream << "App2";
    case ScancodeAudioRewind:
        return stream << "AudioRewind";
    case ScancodeAudioFastForward:
        return stream << "AudioFastForward";
    case ScancodeSoftLeft:
        return stream << "SoftLeft";
    case ScancodeSoftRight:
        return stream << "SoftRight";
    case ScancodeCall:
        return stream << "Call";
    case ScancodeEndCall:
        return stream << "EndCall";
    case ScancodesCount:
        return stream << "ScancodesCount";
    }
    return stream;
}

} // namespace e172
