// Copyright 2023 Borys Boiko

#pragma once

#include "../memcontrol/abstractfactory.h"
#include "../utility/either.h"
#include "../utility/ptr.h"
#include "loadable.h"
#include "loadabletemplate.h"
#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace e172 {

class AbstractAssetExecutor;
class AbstractGraphicsProvider;
class AbstractAudioProvider;
class Context;
class GameApplication;

class AssetProvider : public std::enable_shared_from_this<AssetProvider>
{
    friend GameApplication;
    friend AbstractAssetExecutor;
public:
    AssetProvider() = default;

    struct TemplateNotFound;
    struct TypeNotRegistered;
    struct LoadableCastFailed;

    using Error = std::variant<TemplateNotFound, TypeNotRegistered, LoadableCastFailed>;

    /**
     * template with id not found
     */
    struct TemplateNotFound
    {
        std::string id;

        Error toErr() const { return *this; }
    };

    /**
     * type declated in template not registered
     */
    struct TypeNotRegistered
    {
        std::string typeName;
        std::string templateId;

        Error toErr() const { return *this; }
    };

    /**
     * loadable created but can not be casted to T
     */
    struct LoadableCastFailed
    {
        std::string fromTypeName;
        std::string toTypeName;

        Error toErr() const { return *this; }
    };

    inline friend std::ostream &operator<<(std::ostream &stream, const Error &err)
    {
        return std::visit(e172::Overloaded{
                              [&stream](const TemplateNotFound &err) -> std::ostream & {
                                  return stream << "Template with id " << err.id << " not found.";
                              },
                              [&stream](const TypeNotRegistered &err) -> std::ostream & {
                                  return stream << "Type with name " << err.typeName
                                                << " not registered (requested from template '"
                                                << err.templateId << "').";
                              },
                              [&stream](const LoadableCastFailed &err) -> std::ostream & {
                                  return stream << "Failed to cast loadable of type "
                                                << err.fromTypeName << " to " << err.toTypeName;
                              },
                          },
                          err);
    }

    /**
     * @brief createLoadable - create loadable from template
     * Guarantees that unwrapped Loadable ptr is not null
     * @param loadableTemplate - template to create loadable from
     * @return loadable or error
     */
    Either<Error, Loadable *> createLoadable(const LoadableTemplate &loadableTemplate);

    /**
     * @brief createLoadable - create loadable from template id
     * Guarantees that unwrapped Loadable ptr is not null
     * @param templateId - id to search template in this AssetProvider's context
     * @return loadable or error
     */
    Either<Error, Loadable *> createLoadable(const std::string &templateId);

    /**
     * @brief createLoadable - create loadable from template and cast to T
     * Guarantees that unwrapped Loadable ptr is not null
     * @param loadableTemplate - template to create loadable from
     * @return loadable casted to T or error
     */
    template<typename T>
    auto createLoadable(const LoadableTemplate &loadableTemplate)
    {
        if constexpr (std::is_base_of<Object, T>::value) {
            return createLoadable(loadableTemplate)
                .flatMap<ptr<T>>([](auto obj) -> Either<Error, ptr<T>> {
                    if (const auto res = e172::smart_cast<T>(obj)) {
                        return Right(res);
                    } else {
                        const auto n = obj->m_loadableClassName;
                        delete obj;
                        return Left(
                            LoadableCastFailed{.fromTypeName = n, .toTypeName = Type<T>::name()}
                                .toErr());
                    }
                });
        } else {
            return createLoadable(loadableTemplate).flatMap<T *>([](auto obj) -> Either<Error, T *> {
                if (const auto res = dynamic_cast<T *>(obj)) {
                    return Right(res);
                } else {
                    const auto n = obj->m_loadableClassName;
                    obj.destroy();
                    return Left(LoadableCastFailed{.fromTypeName = n, .toTypeName = Type<T>::name()}
                                    .toErr());
                }
            });
        }
    }

    /**
     * @brief createLoadable - create loadable from template id and cast to T
     * Guarantees that unwrapped Loadable ptr is not null
     * @param templateId - id to search template in this AssetProvider's context
     * @return loadable casted to T or error
     */
    template<typename T>
    auto createLoadable(const std::string &templateId)
    {
        if constexpr (std::is_base_of<Object, T>::value) {
            return createLoadable(templateId).flatMap<ptr<T>>([](auto obj) -> Either<Error, ptr<T>> {
                if (const auto res = e172::smart_cast<T>(obj)) {
                    return Right(res);
                } else {
                    const auto n = obj->m_loadableClassName;
                    delete obj;
                    return Left(LoadableCastFailed{.fromTypeName = n, .toTypeName = Type<T>::name()}
                                    .toErr());
                }
            });
        } else {
            return createLoadable(templateId).flatMap<T *>([](auto obj) -> Either<Error, T *> {
                if (const auto res = dynamic_cast<T *>(obj)) {
                    return Right(res);
                } else {
                    const auto n = obj->m_loadableClassName;
                    obj.destroy();
                    return Left(LoadableCastFailed{.fromTypeName = n, .toTypeName = Type<T>::name()}
                                    .toErr());
                }
            });
        }
    }

    std::vector<std::string> loadableNames();

    void addTemplate(const LoadableTemplate &tmpl);

    /**
     * @brief addDirToSearch - add directory to search assets
     * @param dir
     */
    void addDirToSearch(const std::filesystem::path &path) { m_dirsToSearch.push_back(path); }

    template<typename T>
    void registerType()
        requires std::is_base_of<Loadable, T>::value
    {
        m_factory.registerType<T>();
    }

    void installExecutor(const std::string &id,
                         const std::shared_ptr<AbstractAssetExecutor> &executor);

private:
    void processFile(const std::filesystem::path &file);
    LoadableTemplate parseTemplate(const e172::VariantMap &root, const std::string &path);
    void searchInFolder(const std::filesystem::path &path);

private:
    std::shared_ptr<AbstractGraphicsProvider> m_graphicsProvider;
    std::shared_ptr<AbstractAudioProvider> m_audioProvider;
    Context *m_context = nullptr;
    AbstractFactory<std::string, Loadable> m_factory;
    std::map<std::string, LoadableTemplate> m_templates;
    std::map<std::string, std::shared_ptr<AbstractAssetExecutor>> m_executors;
    std::list<std::filesystem::path> m_dirsToSearch;
};

} // namespace e172
