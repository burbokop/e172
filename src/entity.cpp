#include "entity.h"

#include "net/netsync.h"
#include <optional>
#include <src/math/physicalobject.h>

namespace e172 {

bool Entity::removeTag(const String &tag)
{
    const auto it = m_tags.find(tag);
    if(it != m_tags.end()) {
        m_tags.erase(it);
        return true;
    }
    return false;
}

void Entity::writePhysicsToNet(PhysicalObject &po, WriteBuffer &buf)
{
    buf.write(po.m_rotationKinematics);
    buf.write(po.m_positionKinematics);
    buf.write(po.m_mass);
    buf.write(po.m_friction);
    buf.write(po.m_rotationMatrix);
    buf.write(po.m_blockFrictionPerTick);
    po.m_needSyncNet = false;
}

bool Entity::readPhysicsFromNet(PhysicalObject &po, ReadBuffer &buf)
{
    if (!po.m_rotationKinematics.deserializeAssign(buf)) {
        return false;
    }
    if (!po.m_positionKinematics.deserializeAssign(buf)) {
        return false;
    }
    e172_chainingAssignOrElse(po.m_mass, buf.read<double>(), false);
    e172_chainingAssignOrElse(po.m_friction, buf.read<double>(), false);
    e172_chainingAssignOrElse(po.m_rotationMatrix, buf.read<Matrix>(), false);
    e172_chainingAssignOrElse(po.m_blockFrictionPerTick, buf.read<bool>(), false);
    return true;
}

bool Entity::physicsNeedSyncNet(const PhysicalObject &po)
{
    return po.m_needSyncNet;
}

void Entity::writeNet(WriteBuffer &buf)
{
    for (auto s : m_netSyncs) {
        s->serialize(buf);
        s->wash();
    }

    if (const auto po = dynamic_cast<e172::PhysicalObject *>(this)) {
        writePhysicsToNet(*po, buf);
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
        if (!readPhysicsFromNet(*po, buf)) {
            return false;
        }
    }
    return true;
}

bool Entity::needSyncNet() const
{
    for (const auto &s : m_netSyncs) {
        if (s->dirty()) {
            return true;
        }
    }
    if (const auto po = dynamic_cast<const e172::PhysicalObject *>(this)) {
        if (physicsNeedSyncNet(*po)) {
            return true;
        }
    }
    return false;
}

} // namespace e172
