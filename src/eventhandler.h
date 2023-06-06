#pragma once

#include "abstracteventprovider.h"

namespace e172 {

class EventHandler
{
public:
    EventHandler(AbstractEventProvider *provider);

    bool exitFlag() const;
    bool keyHolded(Scancode key) const;
    bool keySinglePressed(Scancode key) const;
    std::string pullText();
    void update();
    Vector mousePosition() const;
    void enableKeyboard();
    void disableKeyboard();

    AbstractEventProvider *provider() { return m_provider; };
    const AbstractEventProvider *provider() const { return m_provider; };

private:
    void handleEvent(const Event &event);
    static char keySym(Scancode scancode, bool upper);

private:
    static constexpr size_t bufferSize = 512;

    AbstractEventProvider *m_provider;

    bool m_holdedKeys[bufferSize];
    bool m_singlePressedKeys[bufferSize];
    e172::Vector m_mousePosition;
    bool m_exitFlag = false;
    std::string m_textBuffer;
    bool m_keyboardEnabled = true;
};

} // namespace e172
