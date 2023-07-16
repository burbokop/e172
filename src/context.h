// Copyright 2023 Borys Boiko

#pragma once

#include "entity.h"
#include "messagequeue.h"
#include "time/elapsedtimer.h"
#include "utility/observer.h"
#include "utility/ptr.h"
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <vector>

namespace e172 {

class AssetProvider;
class GameApplication;

class Context : public Object
{
    friend GameApplication;

public:
    static inline std::string SettingsFilePath = "./settings.vof";

    using MessageId = Variant;

    enum MessageType : std::uint32_t {
        DestroyEntity = 0,
        DestroyAllEntities,
        DestroyEntitiesWithTag,

        /**
         * UserMessage - for used defined messages
         * Example:
         * ```
         * enum MyMessageType : std::underlying_type<e172::Context::MessageType>::type {
         *     MyType0 = e172::Context::UserMessage,
         *     MyType1,
         *     ...
         * }
         * ```
         */
        UserMessage = 0x1000,
    };

    Context(GameApplication *application);
    std::string absolutePath(const std::string &path) const;
    std::vector<std::string> arguments() const;
    double deltaTime() const { return m_deltaTime; }

    e172::Variant property(const std::string &propertyId) const;
    void setProperty(const std::string &propertyId, const e172::Variant &value);

    Observer<Variant> settingValue(const std::string &id) const;
    void setSettingValue(const std::string &id, const e172::Variant &value);

    std::shared_ptr<AssetProvider> assetProvider() const;
    std::list<ptr<Entity> > entities() const;
    void addEntity(const ptr<Entity> &entity);
    std::shared_ptr<Promice> emitMessage(const MessageId &messageId, const Variant &value = Variant());

    void popMessage(const MessageId &messageId,
                    const std::function<void(Context *, const Variant &)> &callback)
    {
        m_messageQueue.popMessage(messageId, [this, callback](const auto& value) { callback(this, value); });
    }

    template<typename C>
    void popMessage(const MessageId &messageId,
                    C *object,
                    void (C::*callback)(Context *, const Variant &))
    {
        m_messageQueue.popMessage(messageId, [object, this, callback](const auto &value) {
            (object->*callback)(this, value);
        });
    }

    ptr<Entity> entityById(const Entity::Id &id) const;

    ptr<Entity> autoIteratingEntity() const;
    ElapsedTimer::Time proceedDelay() const;
    ElapsedTimer::Time renderDelay() const;

    template<typename T>
    auto entityById(const Entity::Id &id) const
    {
        return e172::smart_cast<T>(entityById(id));
    }

    e172::ptr<e172::Entity> findEntity(const std::function<bool(const e172::ptr<e172::Entity> &)>& condition);

    template<typename T>
    e172::ptr<T> findEntity(const std::function<bool(const e172::ptr<T> &)> &condition)
    {
        return e172::smart_cast<T>(findEntity([condition](const auto &e) {
            e172::ptr<T> castedPtr = e172::smart_cast<T>(e);
            if (castedPtr) {
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

    void later(time_t duration, const std::function<void()> &callback);

    bool quitLater();

private:
    e172::MessageQueue<MessageId, Variant> m_messageQueue;
    double m_deltaTime = 0;
    GameApplication *m_application = nullptr;
    e172::VariantMap m_properties;
    std::map<std::string, Observer<Variant>> m_settings;
};

} // namespace e172
