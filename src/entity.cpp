#include "entity.h"

#include "context.h"
#include "net/netsync.h"

#include <optional>

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
}

void Entity::readNet(ReadBuffer &&buf)
{
    for (auto s : m_netSyncs) {
        s->deserialize(buf);
    }
}

} // namespace e172
