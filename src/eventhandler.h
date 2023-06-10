#pragma once

#include "abstracteventprovider.h"
#include <map>
#include <memory>

namespace e172 {

class EventHandler;

class ClientEventHandler
{
    friend EventHandler;

public:
    bool exitFlag() const
    {
        if (m_keyboardEnabled) {
            return m_exitFlag;
        } else {
            return false;
        }
    }

    bool keyHolded(Scancode key) const
    {
        if (m_keyboardEnabled && key < s_bufferSize)
            return m_holdedKeys[key];

        return false;
    }

    bool keySinglePressed(Scancode key) const
    {
        if (m_keyboardEnabled && key < s_bufferSize)
            return m_singlePressedKeys[key];

        return false;
    }

    std::string pullText();
    Vector mousePosition() const { return m_mousePosition; }
    void enableKeyboard() { m_keyboardEnabled = true; }
    void disableKeyboard() { m_keyboardEnabled = false; }

    ClientEventHandler();

private:
    void handleEvent(const Event &event);
    static char keySym(Scancode scancode, bool upper);

private:
    static constexpr size_t s_bufferSize = 512;

    bool m_holdedKeys[s_bufferSize];
    bool m_singlePressedKeys[s_bufferSize];
    e172::Vector m_mousePosition;
    bool m_exitFlag = false;
    std::string m_textBuffer;
    bool m_keyboardEnabled = true;
};

class EventHandler
{
    friend ClientEventHandler;

public:
    EventHandler(std::shared_ptr<AbstractEventProvider> provider);

    bool exitFlag() const;
    bool keyHolded(Scancode key) const;
    bool keySinglePressed(Scancode key) const;
    std::string pullText();
    void update();
    Vector mousePosition() const;
    void enableKeyboard();
    void disableKeyboard();

    ClientEventHandler *client(PackedClientId clientId)
    {
        const auto it = m_clients.find(clientId);
        if (it != m_clients.end()) {
            return &it->second;
        }
        return nullptr;
    }

    std::shared_ptr<AbstractEventProvider> provider() { return m_provider; };
    std::shared_ptr<const AbstractEventProvider> provider() const { return m_provider; };

private:
    std::shared_ptr<AbstractEventProvider> m_provider;
    std::map<std::optional<PackedClientId>, ClientEventHandler> m_clients;
};

} // namespace e172
