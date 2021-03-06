#include "additional.h"
#include "debug.h"
#include "gameapplication.h"

#include <src/context.h>
#include <src/assettools/assetprovider.h>
#include <src/audio/abstractaudioprovider.h>
#include <src/abstracteventhandler.h>
#include <src/graphics/abstractrenderer.h>
#include <src/graphics/abstractgraphicsprovider.h>
#include <src/time/time.h>

#include <iostream>

namespace e172 {

void __on_sigsegv(int s) {
    const auto st = e172::Debug::stackTrace();
    Debug::warning("Segmentation Fault");
    Debug::warning("Stack trace info:");
    for(auto s : st) {
        Debug::print('\t', s);
    }
    exit(s);
}

size_t GameApplication::static_constructor() {
    e172::Debug::installSigsegvHandler(__on_sigsegv);
    return 0;
}


ElapsedTimer::time_t GameApplication::renderDelay() const {
    return m_renderDelay;
}

e172::ptr<Entity> GameApplication::findEntity(const std::function<bool (const e172::ptr<Entity> &)> &condition) {
    const auto it = std::find_if(m_entities.begin(), m_entities.end(), condition);
    if (it != m_entities.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

void GameApplication::schedule(e172::Time::time_t duration, const std::function<void ()> &function) {
    m_scheduledTasks.push_back({ Time::currentMilliseconds() + duration, function });
}

ElapsedTimer::time_t GameApplication::proceedDelay() const {
    return m_proceedDelay;
}


void GameApplication::destroyAllEntities(Context *, const Variant &) {
    auto it = m_entities.begin();
    while (it != m_entities.end()) {
        if((*it)->liveInHeap()) {
            safeDestroy(*it);
            it = m_entities.erase(it);
        } else {
            ++it;
        }
    }
}

void GameApplication::destroyEntity(Context*, const Variant &value) {
    for(auto it = m_entities.begin(); it != m_entities.end(); ++it) {
        if((*it)->entityId() == value.toNumber<Entity::id_t>()) {
            safeDestroy(*it);
            m_entities.erase(it);
            return;
        }
    }
}

void GameApplication::destroyEntitiesWithTag(Context *, const Variant &value) {
    const auto tag = value.toString();

    auto it = m_entities.begin();
    while (it != m_entities.end()) {
        if((*it)->liveInHeap() && (*it)->containsTag(tag)) {
            safeDestroy(*it);
            it = m_entities.erase(it);
        } else {
            ++it;
        }
    }
}

ptr<Entity> GameApplication::entityInFocus() const {
    return m_entityInFocus;
}

void GameApplication::setEntityInFocus(const ptr<Entity> &entityInFocus) {
    m_entityInFocus = entityInFocus;
}

void GameApplication::render(const ptr<Entity> &entity, AbstractRenderer *renderer) {
    if(entity) {
        if(entity->enabled()) {
            renderer->setDepth(entity->depth());
            entity->render(renderer);
            for(auto euf : entity->__euf) {
                euf.second(entity.data(), renderer);
            }
        }
    }
}

void GameApplication::proceed(const ptr<Entity> &entity, Context *context, AbstractEventHandler *eventHandler) {
    if(entity && context) {
        if(entity->enabled()) {
            bool disableKeyboard;
            if(context->entityInFocus()) {
                disableKeyboard = context->entityInFocus() != entity;
            } else {
                disableKeyboard = !entity->keyboardEnabled();
            }

            if(disableKeyboard) {
                eventHandler->disableKeyboard();
            }
            entity->proceed(context, eventHandler);
            eventHandler->enableKeyboard();
            for(auto euf : entity->__euf) {
                euf.first(entity.data(), context, eventHandler);
            }
        }
    }
}


std::list<ptr<Entity> > GameApplication::entities() const {
    return m_entities;
}

ptr<Entity> GameApplication::autoIteratingEntity() const {
    return m_entities.cyclicValue(nullptr);
}

AbstractGraphicsProvider *GameApplication::graphicsProvider() const {
    return m_graphicsProvider;
}

void GameApplication::setGraphicsProvider(AbstractGraphicsProvider *graphicsProvider) {
    if(graphicsProvider) {
        if(!graphicsProvider->fontLoaded(std::string())) {
            graphicsProvider->loadFont(std::string(), e172::Additional::defaultFont());
        }
    }
    m_graphicsProvider = graphicsProvider;
    m_assetProvider->m_graphicsProvider = graphicsProvider;
}

AbstractAudioProvider *GameApplication::audioProvider() const {
    return m_audioProvider;
}

void GameApplication::setAudioProvider(AbstractAudioProvider *audioProvider) {
    m_audioProvider = audioProvider;
    m_assetProvider->m_audioProvider = audioProvider;
}

void GameApplication::setEventHandler(AbstractEventHandler *eventHandler) {
    m_eventHandler = eventHandler;
}

AbstractEventHandler *GameApplication::eventHandler() const {
    return m_eventHandler;
}

Context *GameApplication::context() const {
    return m_context;
}

AssetProvider *GameApplication::assetProvider() const {
    return m_assetProvider;
}

std::vector<std::string> GameApplication::arguments() const {
    return m_arguments;
}

void GameApplication::registerValueFlag(const std::string &shortName, const std::string &fullName, const std::string &description) {
    m_flagParser.registerValueFlag(shortName, fullName, description);
}

void GameApplication::registerBoolFlag(const std::string &shortName, const std::string &fullName, const std::string &description) {
    m_flagParser.registerBoolFlag(shortName, fullName, description);
}

void GameApplication::displayHelp(std::ostream &stream) {
    m_flagParser.displayHelp(stream);
}

VariantMap GameApplication::flags() const {
    return m_flagParser.flags();
}

bool GameApplication::containsFlag(const std::string &shortName) const {
    return m_flagParser.containsFlag(shortName);
}

Variant GameApplication::flag(const std::string &shortName) const {
    return m_flagParser.flag(shortName);
}

void GameApplication::setRenderInterval(ElapsedTimer::time_t interval) {
    m_renderTimer = ElapsedTimer(interval);
}

void GameApplication::removeApplicationExtension(size_t hash) {
    const auto it = m_applicationExtensions.find(hash);
    if(it != m_applicationExtensions.end())
        m_applicationExtensions.erase(it);
}

GameApplication::GameApplication(int argc, char *argv[]) {
    m_arguments = Additional::coverArgs(argc, argv);
    m_flagParser  = m_arguments;
    m_assetProvider = new AssetProvider();
    m_context = new Context(this);
    m_assetProvider->m_context = m_context;
}

void GameApplication::quitLater() {
    mustQuit = true;
}

int GameApplication::exec() {
    if(m_flagParser.containsFlag("-h")) {
        displayHelp(std::cout);
        return 0;
    }

    for(auto m : m_applicationExtensions) {
        if(m.second->extensionType() == GameApplicationExtension::InitExtension)
            m.second->proceed(this);
    }
    while (1) {
        m_deltaTimeCalculator.update();

        if(m_eventHandler) {
            m_eventHandler->update();
            if(m_eventHandler->exitFlag())
                break;
        }

        e172::ElapsedTimer measureTimer;
        for(const auto& m : m_applicationExtensions) {
            if(m.second->extensionType() == GameApplicationExtension::PreProceedExtension)
                m.second->proceed(this);
        }
        for(const auto& e : m_entities) {
            proceed(e, m_context, m_eventHandler);
        }
        m_proceedDelay = measureTimer.elapsed();
        if(m_graphicsProvider && m_renderTimer.check()) {
            measureTimer.reset();
            auto r = m_graphicsProvider->renderer();
            if(r) {
                r->m_locked = false;
                if(r->m_autoClear) {
                    r->setDepth(std::numeric_limits<int64_t>::min());
                    r->fill(0);
                }
                for(const auto& m : m_applicationExtensions) {
                    if(m.second->extensionType() == GameApplicationExtension::PreRenderExtension)
                        m.second->proceed(this);
                }
                for(const auto& e : m_entities) {
                    render(e, r);
                }
                r->m_locked = true;
                if(!r->update()) {
                    break;
                }
            }
            m_renderDelay = measureTimer.elapsed();
        }

        //AUTO ITERATOR RESET MUST BE BEFORE DESTRUCTION HANDLING
        //if(++m_autoIterator == m_entities.end()) {
        //    m_autoIterator = m_entities.begin();
        //}
        m_entities.nextCycle();

        if(m_context) {
            m_context->popMessage(Context::DESTROY_ENTITY, this, &GameApplication::destroyEntity);
            m_context->popMessage(Context::DESTROY_ALL_ENTITIES, this, &GameApplication::destroyAllEntities);
            m_context->popMessage(Context::DESTROY_ENTITIES_WITH_TAG, this, &GameApplication::destroyEntitiesWithTag);

            m_context->m_messageQueue.invokeInternalFunctions();
            m_context->m_messageQueue.flushMessages();
            m_context->m_deltaTime = m_deltaTimeCalculator.deltaTime();
        }

        {
            auto it = m_scheduledTasks.begin();
            while(it != m_scheduledTasks.end()) {
                if(Time::currentMilliseconds() > it->first) {
                    it->second();
                    it = m_scheduledTasks.erase(it);
                } else {
                    it++;
                }
            }
        }

        if(mustQuit)
            break;
    }
    delete m_assetProvider;
    delete m_context;
    return 0;
}

GameApplicationExtension::ExtensionType GameApplicationExtension::extensionType() const {
    return m_extensionType;
}

void GameApplicationExtension::setExtensionType(const ExtensionType &extensionType) {
    m_extensionType = extensionType;
}

GameApplicationExtension::GameApplicationExtension(GameApplicationExtension::ExtensionType extensionType) {
    m_extensionType = extensionType;
}


}
