// Copyright 2023 Borys Boiko

#include "gameapplication.h"

#include "additional.h"
#include "assettools/assetprovider.h"
#include "audio/abstractaudioprovider.h"
#include "context.h"
#include "debug.h"
#include "entitylifetimeobserver.h"
#include "eventhandler.h"
#include "graphics/abstractgraphicsprovider.h"
#include "graphics/abstractrenderer.h"
#include "time/time.h"
#include "utility/flagparser.h"
#include <iostream>
#include <limits>

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

template<typename C>
void destroyAllEntities(C &container, std::function<void(Entity::Id)> &&removed)
{
    auto it = container.begin();
    while (it != container.end()) {
        if ((*it)->liveInHeap() && !(*it)->liveInSharedPtr()) {
            const auto id = (*it)->entityId();
            destroy(*it);
            it = container.erase(it);
            removed(id);
        } else {
            ++it;
        }
    }
}

template<typename C>
bool destroyEntity(C &container, const Entity::Id id, std::function<void(Entity::Id)> &&removed)
{
    for (auto it = container.begin(); it != container.end(); ++it) {
        if ((*it)->entityId() == id) {
            destroy(*it);
            container.erase(it);
            removed(id);
            return true;
        }
    }
    return false;
}

template<typename C>
void destroyEntitiesWithTag(C &container,
                            const std::string &tag,
                            std::function<void(Entity::Id)> &&removed)
{
    auto it = container.begin();
    while (it != container.end()) {
        if ((*it)->liveInHeap() && (*it)->containsTag(tag)) {
            const auto id = (*it)->entityId();
            destroy(*it);
            it = container.erase(it);
            removed(id);
        } else {
            ++it;
        }
    }
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

void GameApplication::render(const ptr<Entity> &entity, Context *context, AbstractRenderer *renderer)
{
    if (entity) {
        if (entity->enabled()) {
            renderer->setDepth(entity->depth());
            entity->render(context, renderer);
            for (auto euf : entity->__euf) {
                euf.second(entity.data(), context, renderer);
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

            if (disableKeyboard && eventHandler) {
                eventHandler->disableKeyboard();
            }
            entity->proceed(context, eventHandler);
            if (eventHandler) {
                eventHandler->enableKeyboard();
            }
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

bool GameApplication::initRenderer(const std::string &title, const Vector<uint32_t> &resolution)
{
    m_renderer = m_graphicsProvider->createRenderer(title, resolution);
    return m_renderer != nullptr;
}

void GameApplication::setAudioProvider(const std::shared_ptr<AbstractAudioProvider> &audioProvider)
{
    m_audioProvider = audioProvider;
    m_assetProvider->m_audioProvider = audioProvider;
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

GameApplication::GameApplication(const std::vector<std::string> &args)
    : m_arguments(args)
    , m_context(std::make_unique<Context>(this))
    , m_assetProvider(std::make_shared<AssetProvider>())
{
    m_assetProvider->m_context = m_context.get();
}

GameApplication::GameApplication(int argc, const char *argv[])
    : GameApplication(FlagParser::coverArgs(argc, argv))
{}

void GameApplication::quitLater() {
    m_mustQuit = true;
}

int GameApplication::exec()
{
    for (auto m : m_applicationExtensions) {
        if (m.second->extensionType() == GameApplicationExtension::InitExtension)
            m.second->proceed(this);
    }

    /// asset search must be after extensions initialization because thouse may register asset executors
    for (const auto &d : m_assetProvider->m_dirsToSearch) {
        m_assetProvider->searchInFolder(d);
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
        if (!!(m_mode & Mode::Render) && m_renderer && m_renderTimer.check()) {
            e172::ElapsedTimer measureTimer;
            m_renderer->m_locked = false;
            if (m_renderer->m_autoClear) {
                m_renderer->setDepth(std::numeric_limits<int64_t>::min());
                m_renderer->fill(0);
            }
            for (const auto &m : m_applicationExtensions) {
                if (m.second->extensionType() == GameApplicationExtension::PreRenderExtension)
                    m.second->proceed(this);
            }
            for (const auto &e : m_entities) {
                render(e, m_context.get(), m_renderer.get());
            }
            for (const auto &m : m_applicationExtensions) {
                if (m.second->extensionType() == GameApplicationExtension::PostRenderExtension)
                    m.second->proceed(this);
            }
            m_renderer->m_locked = true;
            if (!m_renderer->update()) {
                break;
            }
            for (const auto &m : m_applicationExtensions) {
                if (m.second->extensionType() == GameApplicationExtension::PostPresentExtension)
                    m.second->proceed(this);
            }

            m_renderDelay = measureTimer.elapsed();
        }

        //AUTO ITERATOR RESET MUST BE BEFORE DESTRUCTION HANDLING
        //if(++m_autoIterator == m_entities.end()) {
        //    m_autoIterator = m_entities.begin();
        //}
        m_entities.nextCycle();

        if (m_context) {
            m_context->popMessage(Context::DestroyEntity, [this](Context *, const Variant &value) {
                const auto id = value.toNumber<Entity::Id>();
                destroyEntity(m_entities, id, [this](auto id) { emitEntityRemoved(id); });
            });

            m_context->popMessage(Context::DestroyAllEntities, [this](Context *, const Variant &) {
                destroyAllEntities(m_entities, [this](auto id) { emitEntityRemoved(id); });
            });

            m_context->popMessage(Context::DestroyEntitiesWithTag,
                                  [this](Context *, const Variant &value) {
                                      const auto tag = value.toString();
                                      destroyEntitiesWithTag(m_entities, tag, [this](auto id) {
                                          emitEntityRemoved(id);
                                      });
                                  });

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
