// Copyright 2023 Borys Boiko

#pragma once

#include "meta.h"
#include "object.h"
#include "typedefs.h"
#include <list>
#include <utility>
#include <vector>

namespace e172 {

class Bytes;
class EventHandler;
class AbstractRenderer;
class Context;
class AbstractNetSync;
class GameApplication;
class WriteBuffer;
class ReadBuffer;
class PhysicalObject;
class GameClient;

class Entity : public Object
{
    friend GameApplication;
    friend AbstractNetSync;
    friend GameClient;

public:
    using Id = std::size_t;

    Entity(FactoryMeta &&meta)
        : m_meta(meta.meta())
    {}

    virtual ~Entity() = default;

    virtual void proceed(e172::Context *context, EventHandler *eventHandler) = 0;
    virtual void render(e172::Context *context, AbstractRenderer *renderer) = 0;
    virtual void writeNet(WriteBuffer &buf);
    virtual bool readNet(ReadBuffer &&buf);
    virtual bool needSyncNet() const;

    Id entityId() const { return m_entityId; }
    StringSet tags() const { return m_tags; }
    bool addTag(const String &tag) { return m_tags.insert(tag).second; }
    bool removeTag(const String& tag);
    bool containsTag(const String &tag) { return m_tags.find(tag) != m_tags.end(); }
    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool keyboardEnabled() const { return m_keyboardEnabled; }
    void setKeyboardEnabled(bool keyboardEnabled) { m_keyboardEnabled = keyboardEnabled; }
    int64_t depth() const { return m_depth; }
    void setDepth(const int64_t &depth) { m_depth = depth; }

    const Meta &meta() const { return m_meta; };

private:
    void installNetSync(AbstractNetSync *s) { m_netSyncs.push_back(s); }

    static void writePhysicsToNet(PhysicalObject &po, WriteBuffer &buf);
    static bool readPhysicsFromNet(PhysicalObject &po, ReadBuffer &buf);
    static bool physicsNeedSyncNet(const PhysicalObject &po);

private:
    Meta m_meta;
    static inline Id s_nextId = 0;
    Id m_entityId = ++s_nextId;
    StringSet m_tags;
    bool m_enabled = true;
    bool m_keyboardEnabled = true;
    int64_t m_depth = 0;
    std::vector<AbstractNetSync *> m_netSyncs;

    //[EXPERIMENTAL] extended update functions
private:
    typedef std::pair<void (*)(Entity *, Context *, EventHandler *),
                      void (*)(Entity *, Context *, AbstractRenderer *)>
        __euf_t;
    std::list<__euf_t> __euf;

protected:
    void __installEUF(const __euf_t &euf) { __euf.push_back(euf); }
    //[EXPERIMENTAL END]
};
} // namespace e172
