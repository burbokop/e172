// Copyright 2023 Borys Boiko

#pragma once

#include "variant.h"
#include <string>
#include <utility>
#include <vector>

namespace e172 {

template <typename T, typename V>
T convert_to(const V&);

template<typename T, typename V>
T convert_to(const std::vector<V> &);

template<typename T, typename A, typename B>
T convert_to(const std::pair<A, B> &);

template <typename T>
T convert_to(const VariantList &);

template <typename T>
T convert_to(const VariantVector &);

template <typename T>
T convert_to(const VariantMap &);

template <typename T, typename It>
T convert_to(It, size_t);

template <typename T>
T convert_to(const Variant &);

template <typename T, typename V>
T convert_to(const V& value) {
    if constexpr (std::is_same<T, Variant>::value) {
        return value;
    } else {
        return {};
    }
}

template<typename T, typename V>
T convert_to(const std::vector<V> &vec) {
    if constexpr (std::is_same<T, Variant>::value) {
        VariantList result;
        for (const auto &v : vec) {
            result.push_back(convert_to<Variant>(v));
        }
        return result;
    } else {
        return {};
    }
}

template<typename T, typename A, typename B>
T convert_to(const std::pair<A, B> &pair) {
    if constexpr (std::is_same<T, Variant>::value) {
        return VariantMap {
            { "first", convert_to<Variant>(pair.first) },
            { "second", convert_to<Variant>(pair.second) }
        };
    } else {
        return {};
    }
}

template <typename T>
T convert_to(const VariantList &list) {
    return convert_to<T>(list.begin(), list.size());
}

template <typename T>
T convert_to(const VariantVector &vec) {
    return convert_to<T>(vec.begin(), vec.size());
}

template <typename T, typename It>
T convert_to(It begin, size_t size) {
    constexpr bool is_vector = sfinae::is_specialization<T, std::vector>::value;
    constexpr bool is_list = sfinae::is_specialization<T, std::list>::value;

    if constexpr(is_vector || is_list) {
        typedef typename T::value_type value_type;
        T result;
        if (is_vector)
            result.reserve(size);

        auto it = begin;
        size_t i = 0;
        while (i < size) {
            result.push_back(convert_to<value_type>(*it));
            ++i;
            ++it;
        }
        return result;
    } else {
        return {};
    }
}

template <typename T>
T convert_to(const VariantMap &map) {
    if constexpr(sfinae::is_specialization<T, std::pair>::value) {
        return {
            convert_to<typename T::first_type>(map.at("first")),
            convert_to<typename T::second_type>(map.at("second"))
        };
    } else if constexpr(sfinae::is_specialization<T, std::map>::value) {
        T result;
        for (const auto &p : map) {
            result.insert(result.end(), {
                convert_to<typename T::key_type>(p.first),
                convert_to<typename T::mapped_type>(p.second)
            });
        }
        return result;
    } else {
        return {};
    }
}

template<typename T>
T convert_to(const Variant &variant)
{
    if constexpr (Variant::isNumberType<T>()) {
        return variant.toNumber<T>();
    } else if constexpr (std::is_same<T, std::string>::value) {
        return variant.toString();
    } else if constexpr (sfinae::is_specialization<T, std::vector>::value) {
        return convert_to<T>(variant.toVector());
    } else if constexpr (sfinae::is_specialization<T, std::list>::value) {
        return convert_to<T>(variant.toList());
    } else if constexpr (sfinae::is_specialization<T, std::pair>::value
                         || sfinae::is_specialization<T, std::map>::value) {
        return convert_to<T>(variant.toMap());
    } else {
        return {};
    }
}

} // namespace e172
