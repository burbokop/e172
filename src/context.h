#ifndef CONTEXT_H
#define CONTEXT_H

#include <queue>

#include <src/utility/observer.h>

#include <src/time/elapsedtimer.h>


#include "entity.h"
#include "messagequeue.h"
#include "utility/ptr.h"


namespace e172 {

class AssetProvider;
class GameApplication;
class Context : public Object {
    friend GameApplication;
    double m_deltaTime = 0;
    GameApplication *m_application = nullptr;

    e172::VariantMap m_properties;
    std::map<std::string, Observer<Variant>> m_settings;
public:
    static inline std::string SettingsFilePath = "./settings.vof";

    typedef Variant MessageId;

    enum {
        DESTROY_ENTITY = 0,
        DESTROY_ALL_ENTITIES,
        DESTROY_ENTITIES_WITH_TAG,
        SPAWN_EXPLOSIVE,
        SPAWN_ENGINE_PARTICLES,
        SPAWN_UNIT,
        SPAWN_SURFACE,
        ADD_CAPABILITY,
        REMOVE_CAPABILITY,
        EMERGENCY_MESSAGE,
        BACKGROUND_FLASHING,
        FLOATING_MESSAGE,
        CHANGE_RESOLUTION,
        CHANGE_FULLSCREEN,
        CHANGE_ANAGLYPH
    };

private:
    e172::MessageQueue<MessageId, Variant> m_messageQueue;

public:
    Context(GameApplication *application);
    std::string absolutePath(const std::string &path) const;
    std::vector<std::string> arguments() const;
    double deltaTime() const;

    e172::Variant property(const std::string &propertyId) const;
    void setProperty(const std::string &propertyId, const e172::Variant &value);

    Observer<Variant> settingValue(const std::string &id) const;
    void setSettingValue(const std::string &id, const e172::Variant &value);

    std::shared_ptr<AssetProvider> assetProvider() const;
    std::list<ptr<Entity> > entities() const;
    void addEntity(const ptr<Entity> &entity);
    std::shared_ptr<Promice> emitMessage(const MessageId &messageId, const Variant &value = Variant());

    inline void popMessage(const MessageId &messageId, const std::function<void(Context *, const Variant&)>& callback) {
        m_messageQueue.popMessage(messageId, [this, callback](const auto& value) { callback(this, value); });
    }
    template<typename C>
    void popMessage(const MessageId &messageId, C *object, void(C::*callback)(Context *, const Variant&)) {
        m_messageQueue.popMessage(messageId, [object, this, callback](const auto& value) { (object->*callback)(this, value); });
    }

    ptr<Entity> entityById(const Entity::Id &id) const;

    ptr<Entity> autoIteratingEntity() const;
    ElapsedTimer::time_t proceedDelay() const;
    ElapsedTimer::time_t renderDelay() const;

    template<typename T>
    auto entityById(const Entity::Id &id) const
    {
        return e172::smart_cast<T>(entityById(id));
    }

    e172::ptr<e172::Entity> findEntity(const std::function<bool(const e172::ptr<e172::Entity> &)>& condition);

    template<typename T>
    e172::ptr<T> findEntity(const std::function<bool(const e172::ptr<T> &)>& condition) {
        return e172::smart_cast<T>(findEntity([condition](const auto& e){
            e172::ptr<T> castedPtr = e172::smart_cast<T>(e);
            if(castedPtr) {
                return condition(castedPtr);
            }
            return false;
        }));
    }

    void registerMessageHandler(const MessageId &messageId,
                                const std::function<void(const Vector<double> &)> &callback);

    template<typename C>
    void registerMessageHandler(const MessageId &messageId,
                                C *object,
                                void (C::*callback)(const Vector<double> &))
    {
        registerMessageHandler([messageId, object, callback](auto v){
            (object->*callback)(messageId, v);
        });
    }

    ptr<Entity> entityInFocus() const;
    void setEntityInFocus(const ptr<Entity> &entityInFocus);

    void later(time_t duration, const std::function<void()>& callback);

};


}

#endif // CONTEXT_H
