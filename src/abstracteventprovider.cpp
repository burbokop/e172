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
        return Event::keyDown(buf.read<Scancode>().value());
    case KeyUp:
        return Event::keyUp(buf.read<Scancode>().value());
    case MouseMotion:
        return Event::mouseMotion(buf.read<Vector>().value());
    case Quit:
        return Event::quit();
    }
    throw std::runtime_error("Event::deserialize: Unknown type");
}

} // namespace e172
