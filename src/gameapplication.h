#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

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
class AbstractAudioProvider;
class AbstractGraphicsProvider;
class AssetProvider;
class Context;

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


class GameApplication {
    static size_t static_constructor();
    static const inline size_t static_call = static_constructor();

    std::vector<std::string> m_arguments;

    DeltaTimeCalculator m_deltaTimeCalculator;
    ElapsedTimer m_renderTimer;
    ElapsedTimer::time_t m_proceedDelay = 0;
    ElapsedTimer::time_t m_renderDelay = 0;

    CyclicList<ptr<Entity>> m_entities;
    std::map<size_t, GameApplicationExtension*> m_applicationExtensions;

    AbstractEventHandler *m_eventHandler = nullptr;
    AbstractGraphicsProvider *m_graphicsProvider = nullptr;
    AbstractAudioProvider *m_audioProvider = nullptr;

    AssetProvider *m_assetProvider = nullptr;
    Context *m_context = nullptr;

    void destroyAllEntities(Context*, const Variant&);
    void destroyEntity(Context*, const Variant& value);
    void destroyEntitiesWithTag(Context*, const Variant& value);

    bool mustQuit = false;

    //std::list<Entity*>::iterator m_autoIterator = m_entities.begin();

    std::list<std::pair<e172::Time::time_t, std::function<void()>>> m_scheduledTasks;

    FlagParser m_flagParser;
public:
    GameApplication(int argc, char *argv[]);
    void quitLater();
    int exec();
    std::vector<std::string> arguments() const;

    void registerValueFlag(const std::string& shortName, const std::string& fullName = std::string(), const std::string& description = std::string());
    void registerBoolFlag(const std::string& shortName, const std::string& fullName = std::string(), const std::string& description = std::string());
    void displayHelp(std::ostream& stream);
    VariantMap flags() const;
    bool containsFlag(const std::string &shortName) const;
    Variant flag(const std::string &shortName) const;


    void setRenderInterval(ElapsedTimer::time_t interval);
    inline void addEntity(const ptr<Entity> &entity) { m_entities.push_back(entity); }
    template<typename T>
    inline void addApplicationExtension() {
        const auto it = m_applicationExtensions.find(Type<T>::hash());
        if(it == m_applicationExtensions.end())
            m_applicationExtensions[Type<T>::hash()] = new T();
    }
    template<typename T>
    inline void removeApplicationExtension() {
        removeApplicationExtension(Type<T>::hash());
    }
    void removeApplicationExtension(size_t hash);

    AssetProvider *assetProvider() const;
    e172::Context *context() const;
    AbstractEventHandler *eventHandler() const;
    AbstractAudioProvider *audioProvider() const;
    AbstractGraphicsProvider *graphicsProvider() const;

    void setEventHandler(AbstractEventHandler *eventHandler);
    void setAudioProvider(AbstractAudioProvider *audioProvider);
    void setGraphicsProvider(AbstractGraphicsProvider *graphicsProvider);

    std::list<ptr<Entity>> entities() const;
    ptr<Entity> autoIteratingEntity() const;

    ElapsedTimer::time_t proceedDelay() const;
    ElapsedTimer::time_t renderDelay() const;

    e172::ptr<e172::Entity> findEntity(const std::function<bool(const e172::ptr<e172::Entity> &)>& condition);

    void schedule(e172::Time::time_t duration, const std::function<void()>& function);
};

}
#endif // GAMEAPPLICATION_H
