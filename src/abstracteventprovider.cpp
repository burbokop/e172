#include "abstracteventprovider.h"

namespace e172 {

void Event::serialize(WriteBuffer &buf) const
{
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
    switch (buf.read<Event::Type>().value()) {
    case KeyDown:
        if (const auto &s = buf.read<Scancode>()) {
            return Event::keyDown(*s);
        } else {
            return std::nullopt;
        }
    case KeyUp:
        if (const auto &s = buf.read<Scancode>()) {
            return Event::keyUp(*s);
        } else {
            return std::nullopt;
        }
    case MouseMotion:
        if (const auto &s = buf.read<Vector>()) {
            return Event::mouseMotion(*s);
        } else {
            return std::nullopt;
        }
    case Quit:
        return Event::quit();
    }
    throw std::runtime_error("Event::deserialize: Unknown type");
}

} // namespace e172
