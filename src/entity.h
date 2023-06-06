#pragma once

#include "object.h"
#include "typedefs.h"

#include <src/utility/buffer.h>

namespace e172 {

class Bytes;
class EventHandler;
class AbstractRenderer;
class Context;
class AbstractNetSync;

class Entity : public Object {
    friend class GameApplication;
public:
    typedef uintptr_t id_t;
    Entity() = default;
    virtual ~Entity() = default;

    void virtual proceed(e172::Context *context, EventHandler *eventHandler) = 0;
    void virtual render(AbstractRenderer *renderer) = 0;

    uint64_t entityId() const;
    StringSet tags() const;
    bool addTag(const String& tag);
    bool removeTag(const String& tag);
    bool containsTag(const String& tag);
    bool enabled() const;
    void setEnabled(bool enabled);
    bool keyboardEnabled() const;
    void setKeyboardEnabled(bool keyboardEnabled);
    int64_t depth() const;
    void setDepth(const int64_t &depth);
    void installNetSync(AbstractNetSync *s) { m_netSyncs.push_back(s); }

    bool anyNetSyncDirty() const;

    virtual void writeNet(WriteBuffer &buf);
    virtual void readNet(ReadBuffer &&buf);

private:
    static inline id_t nextId = 0;
    id_t m_entityId = ++nextId;
    StringSet m_tags;
    bool m_enabled = true;
    bool m_keyboardEnabled = true;
    int64_t m_depth = 0;
    std::vector<AbstractNetSync *> m_netSyncs;

    //[EXPERIMENTAL] extended update functions
private:
    typedef std::pair<void (*)(Entity *, Context *, EventHandler *),
                      void (*)(Entity *, AbstractRenderer *)>
        __euf_t;
    std::list<__euf_t> __euf;

protected:
    void __installEUF(const __euf_t &euf) { __euf.push_back(euf); }
    //[EXPERIMENTAL END]
};
} // namespace e172
