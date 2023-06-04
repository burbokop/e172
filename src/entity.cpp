#include "entity.h"

#include "context.h"
#include "net/networker.h"

#include <optional>

namespace e172 {

template<typename It>
requires std::is_same<typename It::value_type, std::uint8_t>::value class WriteIndexMap
{
public:
    WriteIndexMap() = default;

    void add(It begin, It end) {}
    void skip() {}

    std::vector<std::uint8_t> collect() const { todo; }
    bool parse(const std::vector<std::uint8_t> &bytes) { todo; }

private:
};

template<typename It>
requires std::is_same<typename It::value_type, std::uint8_t>::value class ReadIndexMap
{
    using Chunk = std::vector<std::uint8_t>;

public:
    ReadIndexMap(const std::vector<std::uint8_t> &bytes)
        : m_bytes(bytes)
    {}

    std::optional<std::pair<std::size_t, Chunk>> next() { todo; }

private:
    const std::vector<std::uint8_t> &m_bytes;
};

uint64_t Entity::entityId() const
{
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

int64_t Entity::depth() const {
    return m_depth;
}

void Entity::setDepth(const int64_t &depth) {
    m_depth = depth;
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

std::vector<uint8_t> Entity::collectBytes() const
{
    WriteIndexMap<std::vector<std::uint8_t>::const_iterator> map;
    for (auto s : m_netSyncs) {
        if (s->dirty()) {
            const auto b = s->serialize();
            map.add(b.begin(), b.end());
        } else {
            map.skip();
        }
    }
    return map.collect();
}

void Entity::assignBytes(const std::vector<uint8_t> &b) const
{
    ReadIndexMap<std::vector<std::uint8_t>::const_iterator> map(b);
    while (const auto c = map.next()) {
        assert(c->first < m_netSyncs.size());
        m_netSyncs[c->first]->deserAssign(c->second);
    }
}

} // namespace e172
