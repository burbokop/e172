#pragma once

#include "entity.h"

#include <vector>
#include <string>

#include "utility/ptr.h"
#include <src/time/deltatimecalculator.h>
#include <src/time/elapsedtimer.h>
#include <list>
#include <src/type.h>
#include <src/utility/cycliciterator.h>
#include <src/utility/flagparser.h>
#include <src/time/time.h>

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
        PreRenderExtension
    };
    ExtensionType extensionType() const;
protected:
    void setExtensionType(const ExtensionType &extensionType);
private:
    ExtensionType m_extensionType = UndefinedExtension;
public:
    GameApplicationExtension(ExtensionType extensionType);
    virtual void proceed(GameApplication *application) = 0;
};

class GameApplication
{
    static size_t static_constructor();
    static const inline size_t static_call = static_constructor();

    std::vector<std::string> m_arguments;

    DeltaTimeCalculator m_deltaTimeCalculator;
    ElapsedTimer m_renderTimer;
    ElapsedTimer::time_t m_proceedDelay = 0;
    ElapsedTimer::time_t m_renderDelay = 0;

    CyclicList<ptr<Entity>> m_entities;
    std::map<size_t, GameApplicationExtension *> m_applicationExtensions;

    std::unique_ptr<Context> m_context;
    std::unique_ptr<EventHandler> m_eventHandler;

    std::shared_ptr<AbstractEventProvider> m_eventProvider;
    std::shared_ptr<AbstractGraphicsProvider> m_graphicsProvider;
    std::shared_ptr<AbstractAudioProvider> m_audioProvider;
    std::shared_ptr<AssetProvider> m_assetProvider;

    void destroyAllEntities(Context *, const Variant &);
    void destroyEntity(Context*, const Variant& value);
    void destroyEntitiesWithTag(Context*, const Variant& value);

    bool mustQuit = false;

    //std::list<Entity*>::iterator m_autoIterator = m_entities.begin();

    FlagParser m_flagParser;

    ptr<Entity> m_entityInFocus;

public:
    enum class Mode { Proceed = 1 << 0, Render = 1 << 1, All = Proceed | Render };

    inline friend bool operator!(Mode m) { return m == static_cast<Mode>(0); }
    inline friend Mode operator&(Mode l, Mode r) { return Mode(int(l) & int(r)); }
    inline friend Mode operator|(Mode l, Mode r) { return Mode(int(l) & int(r)); }

    GameApplication(int argc, const char *argv[]);
    void quitLater();
    int exec();
    std::vector<std::string> arguments() const;

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

    void setRenderInterval(ElapsedTimer::time_t interval);

    void addEntity(const ptr<Entity> &entity);

    void observeEntityLifeTime(const std::weak_ptr<EntityLifeTimeObserver> &obs)
    {
        m_entityLifeTimeObservers.push_back(obs);
    }

    template<typename T>
    inline void addApplicationExtension()
    {
        const auto it = m_applicationExtensions.find(Type<T>::hash());
        if(it == m_applicationExtensions.end())
            m_applicationExtensions[Type<T>::hash()] = new T();
    }
    template<typename T>
    inline void removeApplicationExtension() {
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

    ElapsedTimer::time_t proceedDelay() const;
    ElapsedTimer::time_t renderDelay() const;

    e172::ptr<e172::Entity> findEntity(
        const std::function<bool(const e172::ptr<e172::Entity> &)> &condition) const;

    e172::ptr<e172::Entity> entityById(Entity::Id id) const
    {
        return findEntity([id](const e172::ptr<e172::Entity> &e) { return e->entityId() == id; });
    }

    void schedule(e172::Time::time_t duration, const std::function<void()>& function);
    void scheduleRepeated(e172::Time::time_t duration, const std::function<void()> &function);

    ptr<Entity> entityInFocus() const;
    void setEntityInFocus(const ptr<Entity> &entityInFocus);

    static void render(const ptr<Entity>& entity, AbstractRenderer* renderer);
    static void proceed(const ptr<Entity> &entity,
                        e172::Context *context,
                        EventHandler *eventHandler);

    Mode mode() const { return m_mode; };
    void setMode(Mode m) { m_mode = m; };

    ~GameApplication();

private:
    void emitEntityAdded(const ptr<Entity> &e);
    void emitEntityRemoved(Entity::Id id);

private:
    struct Task
    {
        bool repeat = false;
        ElapsedTimer timer;
        std::function<void()> proceed;
    };

    std::list<Task> m_scheduledTasks;
    std::list<std::weak_ptr<EntityLifeTimeObserver>> m_entityLifeTimeObservers;

    Mode m_mode = Mode::All;
}; // namespace e172

} // namespace e172
