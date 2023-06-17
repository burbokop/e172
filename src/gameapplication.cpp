// Copyright 2023 Borys Boiko

#include "gameapplication.h"
#include "additional.h"
#include "debug.h"

#include "entitylifetimeobserver.h"
#include <iostream>
#include <limits>
#include <src/assettools/assetprovider.h>
#include <src/audio/abstractaudioprovider.h>
#include <src/context.h>
#include <src/eventhandler.h>
#include <src/graphics/abstractgraphicsprovider.h>
#include <src/graphics/abstractrenderer.h>
#include <src/time/time.h>

namespace e172 {

namespace {

void handleSigsegv(int s)
{
    const auto st = e172::Debug::stackTrace();
    Debug::warning("Segmentation Fault");
    Debug::warning("Stack trace info:");
    for (const auto &s : st) {
        Debug::print('\t', s);
    }
    exit(s);
}

} // namespace

size_t GameApplication::staticConstructor()
{
    e172::Debug::installSigsegvHandler(handleSigsegv);
    return 0;
}

e172::ptr<Entity> GameApplication::findEntity(
    const std::function<bool(const e172::ptr<Entity> &)> &condition) const
{
    const auto it = std::find_if(m_entities.begin(), m_entities.end(), condition);
    if (it != m_entities.end()) {
        return *it;
    } else {
        return nullptr;
    }
}

void GameApplication::schedule(Time::Value duration, const std::function<void()> &function)
{
    m_scheduledTasks.push_back({.repeat = false, .timer = duration, .proceed = function});
}

void GameApplication::scheduleRepeated(Time::Value duration, const std::function<void()> &function)
{
    m_scheduledTasks.push_back({.repeat = true, .timer = duration, .proceed = function});
}

void GameApplication::destroyAllEntities(Context *, const Variant &) {
    auto it = m_entities.begin();
    while (it != m_entities.end()) {
        if ((*it)->liveInHeap() && !(*it)->liveInSharedPtr()) {
            destroy(*it);
            it = m_entities.erase(it);
        } else {
            ++it;
        }
    }
}

void GameApplication::destroyEntity(Context*, const Variant &value) {
    const auto id = value.toNumber<Entity::Id>();
    for (auto it = m_entities.begin(); it != m_entities.end(); ++it) {
        if ((*it)->entityId() == id) {
            destroy(*it);
            m_entities.erase(it);
            emitEntityRemoved(id);
            return;
        }
    }
}

void GameApplication::destroyEntitiesWithTag(Context *, const Variant &value) {
    const auto tag = value.toString();

    auto it = m_entities.begin();
    while (it != m_entities.end()) {
        if ((*it)->liveInHeap() && (*it)->containsTag(tag)) {
            destroy(*it);
            it = m_entities.erase(it);
        } else {
            ++it;
        }
    }
}

void GameApplication::render(const ptr<Entity> &entity, AbstractRenderer *renderer)
{
    if (entity) {
        if (entity->enabled()) {
            renderer->setDepth(entity->depth());
            entity->render(renderer);
            for (auto euf : entity->__euf) {
                euf.second(entity.data(), renderer);
            }
        }
    }
}

void GameApplication::proceed(const ptr<Entity> &entity,
                              Context *context,
                              EventHandler *eventHandler)
{
    if (entity && context) {
        if (entity->enabled()) {
            bool disableKeyboard;
            if (context->entityInFocus()) {
                disableKeyboard = context->entityInFocus() != entity;
            } else {
                disableKeyboard = !entity->keyboardEnabled();
            }

            if (disableKeyboard) {
                eventHandler->disableKeyboard();
            }
            entity->proceed(context, eventHandler);
            eventHandler->enableKeyboard();
            for (auto euf : entity->__euf) {
                euf.first(entity.data(), context, eventHandler);
            }
        }
    }
}

void GameApplication::emitEntityAdded(const ptr<Entity> &e)
{
    auto it = m_entityLifeTimeObservers.begin();
    while (it != m_entityLifeTimeObservers.end()) {
        if (const auto obs = it->lock()) {
            obs->entityAdded(e);
            ++it;
        } else {
            it = m_entityLifeTimeObservers.erase(it);
        }
    }
}

void GameApplication::emitEntityRemoved(Entity::Id id)
{
    auto it = m_entityLifeTimeObservers.begin();
    while (it != m_entityLifeTimeObservers.end()) {
        if (const auto obs = it->lock()) {
            obs->entityRemoved(id);
            ++it;
        } else {
            it = m_entityLifeTimeObservers.erase(it);
        }
    }
}

/// declared in .cpp because unique_ptr
GameApplication::~GameApplication() = default;

ptr<Entity> GameApplication::autoIteratingEntity() const {
    return m_entities.cyclicValue(nullptr);
}

void GameApplication::setEventProvider(const std::shared_ptr<AbstractEventProvider> &eventProvider)
{
    m_eventProvider = eventProvider;

    m_eventHandler = nullptr;
    if (m_eventProvider) {
        m_eventHandler = std::make_unique<EventHandler>(m_eventProvider);
    }
}

void GameApplication::setGraphicsProvider(
    const std::shared_ptr<AbstractGraphicsProvider> &graphicsProvider)
{
    if (graphicsProvider) {
        if (!graphicsProvider->fontLoaded(std::string())) {
            graphicsProvider->loadFont(std::string(), e172::Additional::defaultFont());
        }
    }
    m_graphicsProvider = graphicsProvider;
    m_assetProvider->m_graphicsProvider = graphicsProvider;
}

void GameApplication::setAudioProvider(const std::shared_ptr<AbstractAudioProvider> &audioProvider)
{
    m_audioProvider = audioProvider;
    m_assetProvider->m_audioProvider = audioProvider;
}

void GameApplication::registerValueFlag(const std::string &shortName,
                                        const std::string &fullName,
                                        const std::string &description)
{
    m_flagParser.registerValueFlag(shortName, fullName, description);
}

void GameApplication::registerBoolFlag(const std::string &shortName,
                                       const std::string &fullName,
                                       const std::string &description)
{
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

void GameApplication::addEntity(const ptr<Entity> &entity)
{
    if (entity) {
        m_entities.push_back(entity);
        emitEntityAdded(entity);
    }
}

void GameApplication::removeApplicationExtension(size_t hash) {
    const auto it = m_applicationExtensions.find(hash);
    if (it != m_applicationExtensions.end())
        m_applicationExtensions.erase(it);
}

GameApplication::GameApplication(int argc, const char *argv[]) {
    m_arguments = Additional::coverArgs(argc, argv);
    m_flagParser  = m_arguments;
    m_assetProvider = std::make_shared<AssetProvider>();
    m_context = std::make_unique<Context>(this);
    m_assetProvider->m_context = m_context.get();
}

void GameApplication::quitLater() {
    m_mustQuit = true;
}

int GameApplication::exec()
{
    if (m_flagParser.containsFlag("-h")) {
        displayHelp(std::cout);
        return 0;
    }

    for (auto m : m_applicationExtensions) {
        if (m.second->extensionType() == GameApplicationExtension::InitExtension)
            m.second->proceed(this);
    }
    while (true) {
        if (!!(m_mode & Mode::Proceed) && m_proceedTimer.check()) {
            m_deltaTimeCalculator.update();

            if (m_eventHandler) {
                m_eventHandler->update();
                if (m_eventHandler->exitFlag())
                    break;
            }

            e172::ElapsedTimer measureTimer;
            for (const auto &m : m_applicationExtensions) {
                if (m.second->extensionType() == GameApplicationExtension::PreProceedExtension)
                    m.second->proceed(this);
            }
            for (const auto &e : m_entities) {
                proceed(e, m_context.get(), m_eventHandler.get());
            }
            for (const auto &m : m_applicationExtensions) {
                if (m.second->extensionType() == GameApplicationExtension::PostProceedExtension)
                    m.second->proceed(this);
            }
            m_proceedDelay = measureTimer.elapsed();
        }
        if (!!(m_mode & Mode::Render) && m_graphicsProvider && m_renderTimer.check()) {
            e172::ElapsedTimer measureTimer;
            auto r = m_graphicsProvider->renderer();
            if (r) {
                r->m_locked = false;
                if (r->m_autoClear) {
                    r->setDepth(std::numeric_limits<int64_t>::min());
                    r->fill(0);
                }
                for (const auto &m : m_applicationExtensions) {
                    if (m.second->extensionType() == GameApplicationExtension::PreRenderExtension)
                        m.second->proceed(this);
                }
                for (const auto &e : m_entities) {
                    render(e, r);
                }
                for (const auto &m : m_applicationExtensions) {
                    if (m.second->extensionType() == GameApplicationExtension::PostRenderExtension)
                        m.second->proceed(this);
                }
                r->m_locked = true;
                if (!r->update()) {
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

        if (m_context) {
            m_context->popMessage(Context::DestroyEntity, this, &GameApplication::destroyEntity);
            m_context->popMessage(Context::DestroyAllEntities,
                                  this,
                                  &GameApplication::destroyAllEntities);
            m_context->popMessage(Context::DestroyEntitiesWithTag,
                                  this,
                                  &GameApplication::destroyEntitiesWithTag);

            m_context->m_messageQueue.invokeInternalFunctions();
            m_context->m_messageQueue.flushMessages();
            m_context->m_deltaTime = m_deltaTimeCalculator.deltaTime();
        }

        {
            auto it = m_scheduledTasks.begin();
            while (it != m_scheduledTasks.end()) {
                if (it->timer.check()) {
                    it->proceed();
                    if (it->repeat) {
                        ++it;
                    } else {
                        it = m_scheduledTasks.erase(it);
                    }
                } else {
                    ++it;
                }
            }
        }

        if (m_mustQuit)
            break;
    }
    return 0;
}

} // namespace e172
