// Copyright 2023 Borys Boiko

#pragma once

#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace e172::smartenum {

class Meta
{
public:
    using UnderlyingType = std::uint32_t;

    static Meta fromVaArgs(const std::string &str);

    const std::map<UnderlyingType, std::string> &names() const { return m_names; };

    template<typename T>
    std::optional<std::string> toString(T v) const
        requires std::is_enum<T>::value && (sizeof(T) <= sizeof(UnderlyingType))
    {
        const auto it = m_names.find(static_cast<UnderlyingType>(v));
        if (it != m_names.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

    template<typename T>
    std::ostream &write(std::ostream &stream, T v) const
        requires std::is_enum<T>::value && (sizeof(T) <= sizeof(UnderlyingType))
    {
        const auto it = m_names.find(static_cast<UnderlyingType>(v));
        if (it != m_names.end()) {
            return stream << it->second;
        } else {
            return stream;
        }
    }

private:
    Meta(const std::map<UnderlyingType, std::string> &names)
        : m_names(names)
    {}

private:
    std::map<UnderlyingType, std::string> m_names;
};

} // namespace e172::smartenum

#define e172_enum(TYPE, ...) \
public: \
    enum TYPE { __VA_ARGS__ }; \
\
private: \
    static inline const auto TYPE##Meta = e172::smartenum::Meta::fromVaArgs(#__VA_ARGS__); \
\
public: \
    friend inline std::ostream &operator<<(std::ostream &stream, TYPE value) \
    { \
        return TYPE##Meta.write(stream, value); \
    } \
\
private: \
    ; // NOLINT

#define e172_enum_class(TYPE, ...) \
    enum class TYPE { __VA_ARGS__ }; \
    inline const auto TYPE##Meta = e172::smartenum::Meta::fromVaArgs(#__VA_ARGS__); \
    inline std::ostream &operator<<(std::ostream &stream, TYPE value) \
    { \
        return TYPE##Meta.write(stream, value); \
    }

#define e172_enum_member(TYPE, NAME, ...) \
public: \
    enum TYPE { __VA_ARGS__ }; \
\
private: \
    TYPE m_##NAME; \
    static inline const auto TYPE##Meta = e172::smartenum::Meta::fromVaArgs(#__VA_ARGS__); \
\
public: \
    TYPE NAME() const \
    { \
        return m_##NAME; \
    } \
    auto NAME##String() const \
    { \
        return TYPE##Meta.toString(m_##NAME).value_or(std::string()); \
    } \
\
private: \
    ; // NOLINT
