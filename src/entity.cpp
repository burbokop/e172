#include "entity.h"

#include "context.h"

namespace e172 {

uint64_t Entity::entityId() const {
    return m_entityId;
}

StringSet Entity::tags() const {
    return m_tags;
}

bool Entity::addTag(const String &tag) {
    return m_tags.insert(tag).second;
}

bool Entity::removeTag(const String &tag) {
    const auto it = m_tags.find(tag);
    if(it != m_tags.end()) {
        m_tags.erase(it);
        return true;
    }
    return false;
}

bool Entity::containsTag(const String &tag) {
    return m_tags.find(tag) != m_tags.end();
}

bool Entity::enabled() const {
    return m_enabled;
}

void Entity::setEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Entity::keyboardEnabled() const {
    return m_keyboardEnabled;
}

void Entity::setKeyboardEnabled(bool keyboardEnabled) {
    m_keyboardEnabled = keyboardEnabled;
}

int64_t Entity::depth() const
{
    return m_depth;
}

void Entity::setDepth(const int64_t &depth)
{
    m_depth = depth;
}

Entity::Entity() {}

Entity::~Entity() {}


}
