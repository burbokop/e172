// Copyright 2023 Borys Boiko

#pragma once

#include <ostream>

namespace e172 {
// Deprecated use stl concepts
namespace sfinae {

//Deprecated. Use StreamSperator instead.
namespace StreamOperator_old {

struct no
{
    bool b[2];
};

template<typename T, typename Arg>
no operator<<(T &, const Arg &);

bool check(...);
no &check(const no &);

template<typename T, typename Arg = T>
struct exists
{
    enum { value = (sizeof(check(*(T *) (0) << *(Arg *) (0))) != sizeof(no)) }; // NOLINT
};

} // namespace StreamOperator_old

template<typename StreamType, typename Type>
class StreamOperator
{
    static StreamType stream_val;

    template<typename U, typename SFINAE = decltype(stream_val << std::declval<U>())>
    constexpr static bool test(int) // NOLINT
    {
        return true;
    }

    template<typename U>
    constexpr static bool test(...)
    {
        return false;
    }

public:
    constexpr static bool value = test<Type>(0);
};

template <typename T, typename DestinationType>
class TypeConvertionOperator {
    typedef char one;
    typedef long two; // NOLINT

    template<typename C>
    static one test(decltype(&C::operator DestinationType)); // NOLINT
    template <typename C> static two test(...);

public:
    enum { value = sizeof(test<T>(0)) == sizeof(one) };
};

namespace EquealOperator {

struct No { bool b[2]; };
template<typename T, typename Arg> No operator== (const T&, const Arg&);

bool Check(...);       // NOLINT
No &Check(const No &); // NOLINT

template <typename T, typename Arg = T>
struct exists {
    enum { value = (sizeof(Check(*(T *) (0) == *(Arg *) (0))) != sizeof(No)) }; // NOLINT
};

} // namespace EquealOperator

namespace LessOperator {

struct No { bool b[2]; };
template<typename T, typename Arg> No operator< (const T&, const Arg&);

bool Check(...);       // NOLINT
No &Check(const No &); // NOLINT

template <typename T, typename Arg = T>
struct exists {
    enum { value = (sizeof(Check(*(T *) (0) < *(Arg *) (0))) != sizeof(No)) }; // NOLINT
};

} // namespace LessOperator

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type
{};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref>: std::true_type {};

} // namespace sfinae
} // namespace e172

#define E172_SFINAE_METHOD_CHECKER(METHOD_NAME) \
    template<typename E172_SFINAE_##METHOD_NAME##_T> \
    class has_##METHOD_NAME##_method \
    { \
    private: \
        typedef char YesType[1]; \
        typedef char NoType[2]; \
        template<typename E172_SFINAE_##METHOD_NAME##_C> \
        static YesType &test(decltype(&E172_SFINAE_##METHOD_NAME##_C::METHOD_NAME)); \
        template<typename E172_SFINAE_##METHOD_NAME##_C> \
        static NoType &test(...); \
\
    public: \
        enum { value = sizeof(test<E172_SFINAE_##METHOD_NAME##_T>(0)) == sizeof(YesType) }; \
    };
