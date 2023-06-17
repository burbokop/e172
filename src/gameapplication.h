// Copyright 2023 Borys Boiko

#pragma once

#include "entity.h"
#include "utility/ptr.h"
#include <list>
#include <map>
#include <memory>
#include <src/time/deltatimecalculator.h>
#include <src/time/elapsedtimer.h>
#include <src/time/time.h>
#include <src/type.h>
#include <src/utility/cycliciterator.h>
#include <src/utility/flagparser.h>
#include <string>
#include <vector>

namespace e172 {

class Time;
class EventHandler;
class AbstractEventProvider;
class AbstractAudioProvider;
class AbstractGraphicsProvider;
class AssetProvider;
class Context;
class EntityLifeTimeObserver;
class GameApplication;

class GameApplicationExtension {
public:
    enum ExtensionType {
        UndefinedExtension,
        InitExtension,
        PreProceedExtension,
        PreRenderExtension,
        PostProceedExtension,
        PostRenderExtension
    };

    GameApplicationExtension(ExtensionType extensionType)
        : m_extensionType(extensionType)
    {}

    ExtensionType extensionType() const { return m_extensionType; }
    virtual void proceed(GameApplication *application) = 0;

protected:
    void setExtensionType(const ExtensionType &extensionType) { m_extensionType = extensionType; }

private:
    ExtensionType m_extensionType = UndefinedExtension;
};

class GameApplication
{
public:
    enum class Mode { Proceed = 1 << 0, Render = 1 << 1, All = Proceed | Render };

    inline friend bool operator!(Mode m) { return m == static_cast<Mode>(0); }

    inline friend Mode operator&(Mode l, Mode r)
    {
        return Mode(static_cast<int>(l) & static_cast<int>(r));
    }
    inline friend Mode operator|(Mode l, Mode r)
    {
        return Mode(static_cast<int>(l) & static_cast<int>(r));
    }

    GameApplication(int argc, const char *argv[]);

    void quitLater();
    int exec();

    std::vector<std::string> arguments() const { return m_arguments; }

    void registerValueFlag(const std::string &shortName,
                           const std::string &fullName = std::string(),
                           const std::string &description = std::string());

    void registerBoolFlag(const std::string &shortName,
                          const std::string &fullName = std::string(),
                          const std::string &description = std::string());

    void displayHelp(std::ostream &stream);
    VariantMap flags() const;
    bool containsFlag(const std::string &shortName) const;
    Variant flag(const std::string &shortName) const;

    void setRenderInterval(ElapsedTimer::Time interval) { m_renderTimer = interval; }
    void setProccedInterval(ElapsedTimer::Time interval) { m_proceedTimer = interval; }

    void addEntity(const ptr<Entity> &entity);

    void observeEntityLifeTime(const std::weak_ptr<EntityLifeTimeObserver> &obs)
    {
        m_entityLifeTimeObservers.push_back(obs);
    }

    template<typename T>
    void addApplicationExtension()
    {
        const auto it = m_applicationExtensions.find(Type<T>::hash());
        if (it == m_applicationExtensions.end())
            m_applicationExtensions[Type<T>::hash()] = new T();
    }

    template<typename T>
    void removeApplicationExtension()
    {
        removeApplicationExtension(Type<T>::hash());
    }

    void removeApplicationExtension(size_t hash);

    e172::Context *context() const { return m_context.get(); }
    EventHandler *eventHandler() const { return m_eventHandler.get(); }

    std::shared_ptr<AbstractEventProvider> eventProvider() const { return m_eventProvider; }
    std::shared_ptr<AbstractAudioProvider> audioProvider() const { return m_audioProvider; }

    std::shared_ptr<AbstractGraphicsProvider> graphicsProvider() const
    {
        return m_graphicsProvider;
    }

    std::shared_ptr<AssetProvider> assetProvider() const { return m_assetProvider; }

    void setEventProvider(const std::shared_ptr<AbstractEventProvider> &eventProvider);
    void setAudioProvider(const std::shared_ptr<AbstractAudioProvider> &audioProvider);
    void setGraphicsProvider(const std::shared_ptr<AbstractGraphicsProvider> &graphicsProvider);

    const std::list<ptr<Entity>> &entities() const { return m_entities; }
    ptr<Entity> autoIteratingEntity() const;

    ElapsedTimer::Time proceedDelay() const { return m_proceedDelay; }
    ElapsedTimer::Time renderDelay() const { return m_renderDelay; }

    e172::ptr<e172::Entity> findEntity(
        const std::function<bool(const e172::ptr<e172::Entity> &)> &condition) const;

    e172::ptr<e172::Entity> entityById(Entity::Id id) const
    {
        return findEntity([id](const e172::ptr<e172::Entity> &e) { return e->entityId() == id; });
    }

    void schedule(e172::Time::Value duration, const std::function<void()> &function);
    void scheduleRepeated(e172::Time::Value duration, const std::function<void()> &function);

    ptr<Entity> entityInFocus() const { return m_entityInFocus; }
    void setEntityInFocus(const ptr<Entity> &entityInFocus) { m_entityInFocus = entityInFocus; }

    static void render(const ptr<Entity>& entity, AbstractRenderer* renderer);
    static void proceed(const ptr<Entity> &entity,
                        e172::Context *context,
                        EventHandler *eventHandler);

    Mode mode() const { return m_mode; };
    void setMode(Mode m) { m_mode = m; };

    ~GameApplication();

private:
    struct Task
    {
        bool repeat = false;
        ElapsedTimer timer;
        std::function<void()> proceed;
    };

    void emitEntityAdded(const ptr<Entity> &e);
    void emitEntityRemoved(Entity::Id id);

    static size_t staticConstructor();
    void destroyAllEntities(Context *, const Variant &);
    void destroyEntity(Context *, const Variant &value);
    void destroyEntitiesWithTag(Context *, const Variant &value);

private:
    static const inline size_t s_staticCall = staticConstructor();

    std::vector<std::string> m_arguments;

    DeltaTimeCalculator<std::chrono::high_resolution_clock> m_deltaTimeCalculator;
    ElapsedTimer m_renderTimer;
    ElapsedTimer m_proceedTimer;
    ElapsedTimer::Time m_proceedDelay = 0;
    ElapsedTimer::Time m_renderDelay = 0;

    CyclicList<ptr<Entity>> m_entities;
    std::map<size_t, GameApplicationExtension *> m_applicationExtensions;

    std::unique_ptr<Context> m_context;
    std::unique_ptr<EventHandler> m_eventHandler;

    std::shared_ptr<AbstractEventProvider> m_eventProvider;
    std::shared_ptr<AbstractGraphicsProvider> m_graphicsProvider;
    std::shared_ptr<AbstractAudioProvider> m_audioProvider;
    std::shared_ptr<AssetProvider> m_assetProvider;

    bool m_mustQuit = false;

    //std::list<Entity*>::iterator m_autoIterator = m_entities.begin();

    FlagParser m_flagParser;

    ptr<Entity> m_entityInFocus;

    std::list<Task> m_scheduledTasks;
    std::list<std::weak_ptr<EntityLifeTimeObserver>> m_entityLifeTimeObservers;

    Mode m_mode = Mode::All;
};

} // namespace e172
