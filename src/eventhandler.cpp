#include "eventhandler.h"

namespace e172 {

ClientEventHandler::ClientEventHandler()
{
    for (size_t i = 0; i < s_bufferSize; i++) {
        m_singlePressedKeys[i] = false;
        m_holdedKeys[i] = false;
    }
}

EventHandler::EventHandler(std::shared_ptr<AbstractEventProvider> provider)
    : m_provider(std::move(provider))
{
    assert(m_provider);
}

void ClientEventHandler::handleEvent(const Event &event)
{
    if (event.type() == Event::KeyDown) {
        const auto scancode = event.scancode().value();
        if (scancode < s_bufferSize) {
            m_holdedKeys[scancode] = true;
            m_singlePressedKeys[scancode] = true;

            if (scancode != ScancodeLShift) {
                if (m_holdedKeys[ScancodeLShift]) {
                    m_textBuffer.append(1, keySym(scancode, true));
                } else {
                    m_textBuffer.append(1, keySym(scancode, false));
                }
            }
        }
    } else if (event.type() == Event::KeyUp) {
        const auto scancode = event.scancode().value();
        if (scancode < s_bufferSize) {
            m_holdedKeys[scancode] = false;
        }
    } else if (event.type() == Event::MouseMotion) {
        m_mousePosition = event.pos().value();
    } else if (event.type() == Event::Quit) {
        m_exitFlag = true;
    }
}

bool EventHandler::exitFlag() const
{
    for (auto &client : m_clients) {
        if (client.second.exitFlag()) {
            return true;
        }
    }
    return false;
}

bool EventHandler::keyHolded(e172::Scancode key) const
{
    for (auto &client : m_clients) {
        if (client.second.keyHolded(key)) {
            return true;
        }
    }
    return false;
}

bool EventHandler::keySinglePressed(e172::Scancode key) const
{
    for (auto &client : m_clients) {
        if (client.second.keySinglePressed(key)) {
            return true;
        }
    }
    return false;
}

void EventHandler::update()
{
    for (auto &client : m_clients) {
        for (size_t i = 0; i < ClientEventHandler::s_bufferSize; i++) {
            client.second.m_singlePressedKeys[i] = false;
        }
    }
    if (m_provider) {
        while (const auto &event = m_provider->pullEvent()) {
            m_clients[event->clientId()].handleEvent(*event);
        }
    }
}

e172::Vector EventHandler::mousePosition() const
{
    if (m_clients.empty())
        return Vector();

    Vector result;
    for (auto &client : m_clients) {
        result += client.second.mousePosition();
    }
    return result / m_clients.size();
}

std::string ClientEventHandler::pullText()
{
    if (m_keyboardEnabled) {
        auto result = m_textBuffer;
        m_textBuffer.clear();
        return result;
    } else {
        return std::string();
    }
}

std::string EventHandler::pullText()
{
    for (auto &client : m_clients) {
        const auto text = client.second.pullText();
        if (!text.empty()) {
            return text;
        }
    }
    return std::string();
}

void EventHandler::enableKeyboard()
{
    for (auto &client : m_clients) {
        client.second.m_keyboardEnabled = true;
    }
}

void EventHandler::disableKeyboard()
{
    for (auto &client : m_clients) {
        client.second.m_keyboardEnabled = false;
    }
}

char ClientEventHandler::keySym(Scancode scancode, bool upper)
{
    switch (scancode) {
    case ScancodeUnknown:
        return 0;
    case ScancodeA:
        return upper ? 'A' : 'a';
    case ScancodeB:
        return upper ? 'B' : 'b';
    case ScancodeC:
        return upper ? 'C' : 'c';
    case ScancodeD:
        return upper ? 'D' : 'd';
    case ScancodeE:
        return upper ? 'E' : 'e';
    case ScancodeF:
        return upper ? 'F' : 'f';
    case ScancodeG:
        return upper ? 'G' : 'g';
    case ScancodeH:
        return upper ? 'H' : 'h';
    case ScancodeI:
        return upper ? 'I' : 'i';
    case ScancodeJ:
        return upper ? 'J' : 'j';
    case ScancodeK:
        return upper ? 'K' : 'k';
    case ScancodeL:
        return upper ? 'L' : 'l';
    case ScancodeM:
        return upper ? 'M' : 'm';
    case ScancodeN:
        return 0;
    case ScancodeO:
        return 0;
    case ScancodeP:
        return 0;
    case ScancodeQ:
        return 0;
    case ScancodeR:
        return 0;
    case ScancodeS:
        return 0;
    case ScancodeT:
        return 0;
    case ScancodeU:
        return 0;
    case ScancodeV:
        return 0;
    case ScancodeW:
        return 0;
    case ScancodeX:
        return 0;
    case ScancodeY:
        return 0;
    case ScancodeZ:
        return 0;
    case Scancode1:
        return 0;
    case Scancode2:
        return 0;
    case Scancode3:
        return 0;
    case Scancode4:
        return 0;
    case Scancode5:
        return 0;
    case Scancode6:
        return 0;
    case Scancode7:
        return 0;
    case Scancode8:
        return 0;
    case Scancode9:
        return 0;
    case Scancode0:
        return 0;
    case ScancodeReturn:
        return 0;
    case ScancodeEscape:
        return 0;
    case ScancodeBackSpace:
        return 0;
    case ScancodeTab:
        return 0;
    case ScancodeSpace:
        return 0;
    case ScancodeMinus:
        return 0;
    case ScancodeEquals:
        return 0;
    case ScancodeLeftBracket:
        return 0;
    case ScancodeRightBracket:
        return 0;
    case ScancodeBackslash:
        return 0;
    case ScancodeNonushash:
        return 0;
    case ScancodeSemicolon:
        return 0;
    case ScancodeApostrophe:
        return 0;
    case ScancodeGrave:
        return 0;
    case ScancodeComma:
        return 0;
    case ScancodePeriod:
        return 0;
    case ScancodeSlash:
        return 0;
    case ScancodeCapsLock:
        return 0;
    case ScancodeF1:
        return 0;
    case ScancodeF2:
        return 0;
    case ScancodeF3:
        return 0;
    case ScancodeF4:
        return 0;
    case ScancodeF5:
        return 0;
    case ScancodeF6:
        return 0;
    case ScancodeF7:
        return 0;
    case ScancodeF8:
        return 0;
    case ScancodeF9:
        return 0;
    case ScancodeF10:
        return 0;
    case ScancodeF11:
        return 0;
    case ScancodeF12:
        return 0;
    case ScancodePrintScreen:
        return 0;
    case ScancodeScrollLock:
        return 0;
    case ScancodePause:
        return 0;
    case ScancodeInsert:
        return 0;
    case ScancodeHome:
        return 0;
    case ScancodePageUp:
        return 0;
    case ScancodeDelete:
        return 0;
    case ScancodeEnd:
        return 0;
    case ScancodePageDown:
        return 0;
    case ScancodeRight:
        return 0;
    case ScancodeLeft:
        return 0;
    case ScancodeDown:
        return 0;
    case ScancodeUp:
        return 0;
    case ScancodeNumLockClear:
        return 0;
    case ScancodeKpDivide:
        return 0;
    case ScancodeKpMultiply:
        return 0;
    case ScancodeKpMinus:
        return 0;
    case ScancodeKpPlus:
        return 0;
    case ScancodeKpEnter:
        return 0;
    case ScancodeKp1:
        return 0;
    case ScancodeKp2:
        return 0;
    case ScancodeKp3:
        return 0;
    case ScancodeKp4:
        return 0;
    case ScancodeKp5:
        return 0;
    case ScancodeKp6:
        return 0;
    case ScancodeKp7:
        return 0;
    case ScancodeKp8:
        return 0;
    case ScancodeKp9:
        return 0;
    case ScancodeKp0:
        return 0;
    case ScancodeKpPeriod:
        return 0;
    case ScancodeNonUsBackSlash:
        return 0;
    case ScancodeApplication:
        return 0;
    case ScancodePower:
        return 0;
    case ScancodeKPEquals:
        return 0;
    case ScancodeF13:
        return 0;
    case ScancodeF14:
        return 0;
    case ScancodeF15:
        return 0;
    case ScancodeF16:
        return 0;
    case ScancodeF17:
        return 0;
    case ScancodeF18:
        return 0;
    case ScancodeF19:
        return 0;
    case ScancodeF20:
        return 0;
    case ScancodeF21:
        return 0;
    case ScancodeF22:
        return 0;
    case ScancodeF23:
        return 0;
    case ScancodeF24:
        return 0;
    case ScancodeExecute:
        return 0;
    case ScancodeHelp:
        return 0;
    case ScancodeMenu:
        return 0;
    case ScancodeSelect:
        return 0;
    case ScancodeStop:
        return 0;
    case ScancodeAgain:
        return 0;
    case ScancodeUndo:
        return 0;
    case ScancodeCut:
        return 0;
    case ScancodeCopy:
        return 0;
    case ScancodePaste:
        return 0;
    case ScancodeFind:
        return 0;
    case ScancodeMute:
        return 0;
    case ScancodeVolumeUp:
        return 0;
    case ScancodeVolumeDown:
        return 0;
    case ScancodeKPComma:
        return 0;
    case ScancodeKPEqualSas400:
        return 0;
    case ScancodeInternational1:
        return 0;
    case ScancodeInternational2:
        return 0;
    case ScancodeInternational3:
        return 0;
    case ScancodeInternational4:
        return 0;
    case ScancodeInternational5:
        return 0;
    case ScancodeInternational6:
        return 0;
    case ScancodeInternational7:
        return 0;
    case ScancodeInternational8:
        return 0;
    case ScancodeInternational9:
        return 0;
    case ScancodeLang1:
        return 0;
    case ScancodeLang2:
        return 0;
    case ScancodeLang3:
        return 0;
    case ScancodeLang4:
        return 0;
    case ScancodeLang5:
        return 0;
    case ScancodeLang6:
        return 0;
    case ScancodeLang7:
        return 0;
    case ScancodeLang8:
        return 0;
    case ScancodeLang9:
        return 0;
    case ScancodeAlterase:
        return 0;
    case ScancodeSysREQ:
        return 0;
    case ScancodeCancel:
        return 0;
    case ScancodeClear:
        return 0;
    case ScancodePrior:
        return 0;
    case ScancodeReturn2:
        return 0;
    case ScancodeSeparator:
        return 0;
    case ScancodeOut:
        return 0;
    case ScancodeOper:
        return 0;
    case ScancodeClearAgain:
        return 0;
    case ScancodeCrsel:
        return 0;
    case ScancodeExsel:
        return 0;
    case ScancodeKP00:
        return 0;
    case ScancodeKP000:
        return 0;
    case ScancodeThousandsSeparator:
        return 0;
    case ScancodeDecimalSeparator:
        return 0;
    case ScancodeCurrencyUnit:
        return 0;
    case ScancodeCurrencySubunit:
        return 0;
    case ScancodeKpLeftParen:
        return 0;
    case ScancodeKpRightParen:
        return 0;
    case ScancodeKpLeftBrace:
        return 0;
    case ScancodeKpRightBrace:
        return 0;
    case ScancodeKpTab:
        return 0;
    case ScancodeKpBackspace:
        return 0;
    case ScancodeKpA:
        return 0;
    case ScancodeKpB:
        return 0;
    case ScancodeKpC:
        return 0;
    case ScancodeKpD:
        return 0;
    case ScancodeKpE:
        return 0;
    case ScancodeKpF:
        return 0;
    case ScancodeKpXor:
        return 0;
    case ScancodeKpPower:
        return 0;
    case ScancodeKpPercent:
        return 0;
    case ScancodeKpLess:
        return 0;
    case ScancodeKpGreater:
        return 0;
    case ScancodeKpAmpersand:
        return 0;
    case ScancodeKpDblAmpersand:
        return 0;
    case ScancodeKpVerticalBar:
        return 0;
    case ScancodeKpDblVerticalBar:
        return 0;
    case ScancodeKpColon:
        return 0;
    case ScancodeKpHash:
        return 0;
    case ScancodeKpSpace:
        return 0;
    case ScancodeKpAt:
        return 0;
    case ScancodeKpExclam:
        return 0;
    case ScancodeKpMemStore:
        return 0;
    case ScancodeKpMemRecall:
        return 0;
    case ScancodeKpMemClear:
        return 0;
    case ScancodeKpMemAdd:
        return 0;
    case ScancodeKpMemSubtract:
        return 0;
    case ScancodeKpMemMultiply:
        return 0;
    case ScancodeKpMemDivide:
        return 0;
    case ScancodeKpPlusMinus:
        return 0;
    case ScancodeKpClear:
        return 0;
    case ScancodeKpClearEntry:
        return 0;
    case ScancodeKpBinary:
        return 0;
    case ScancodeKpOctal:
        return 0;
    case ScancodeKpDecimal:
        return 0;
    case ScancodeKpHexadecimal:
        return 0;
    case ScancodeLCtrl:
        return 0;
    case ScancodeLShift:
        return 0;
    case ScancodeLAlt:
        return 0;
    case ScancodeLGui:
        return 0;
    case ScancodeRCtrl:
        return 0;
    case ScancodeRShift:
        return 0;
    case ScancodeRAlt:
        return 0;
    case ScancodeRGui:
        return 0;
    case ScancodeMode:
        return 0;
    case ScancodeAudioNext:
        return 0;
    case ScancodeAudioPrev:
        return 0;
    case ScancodeAudioStop:
        return 0;
    case ScancodeAudioPlay:
        return 0;
    case ScancodeAudioMute:
        return 0;
    case ScancodeMediaSelect:
        return 0;
    case ScancodeWWW:
        return 0;
    case ScancodeMail:
        return 0;
    case ScancodeCalculator:
        return 0;
    case ScancodeComputer:
        return 0;
    case ScancodeAcSearch:
        return 0;
    case ScancodeAcHome:
        return 0;
    case ScancodeAcBack:
        return 0;
    case ScancodeAcForward:
        return 0;
    case ScancodeAcStop:
        return 0;
    case ScancodeAcRefresh:
        return 0;
    case ScancodeAcBookmarks:
        return 0;
    case ScancodeBrightnessDown:
        return 0;
    case ScancodeBrightnessUp:
        return 0;
    case ScancodeDisplaySwitch:
        return 0;
    case ScancodeKbDillumToggle:
        return 0;
    case ScancodeKbDillumDown:
        return 0;
    case ScancodeKbDillumUp:
        return 0;
    case ScancodeEject:
        return 0;
    case ScancodeSleep:
        return 0;
    case ScancodeApp1:
        return 0;
    case ScancodeApp2:
        return 0;
    case ScancodeAudioRewind:
        return 0;
    case ScancodeAudioFastForward:
        return 0;
    case ScancodesCount:
        return 0;
    }
    return 0;
}

} // namespace e172
