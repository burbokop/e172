#include "entity.h"

#include "context.h"
#include "net/netsync.h"

#include <optional>

#include <src/math/physicalobject.h>

namespace e172 {

bool Entity::removeTag(const String &tag) {
    const auto it = m_tags.find(tag);
    if(it != m_tags.end()) {
        m_tags.erase(it);
        return true;
    }
    return false;
}

bool Entity::anyNetSyncDirty() const
{
    for (const auto &s : m_netSyncs) {
        if (s->dirty()) {
            return true;
        }
    }
    return false;
}

void Entity::writeNet(WriteBuffer &buf)
{
    for (auto s : m_netSyncs) {
        s->serialize(buf);
    }
    if (const auto po = dynamic_cast<e172::PhysicalObject *>(this)) {
        po->writeNet(buf);
    }
}

bool Entity::readNet(ReadBuffer &&buf)
{
    for (auto s : m_netSyncs) {
        if (!s->deserialize(buf)) {
            return false;
        }
    }
    if (const auto po = dynamic_cast<e172::PhysicalObject *>(this)) {
        if (!po->readNet(buf)) {
            return false;
        }
    }
    return true;
}

} // namespace e172
