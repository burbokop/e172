// Copyright 2023 Borys Boiko

#pragma once

#define E172_DISABLE_VARIANT_ABSTRACT_CONSTRUCTOR
#define E172_USE_VARIANT_RTTI_OBJECT

#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "math/vector.h"
#include "sfinae.h"
#include "type.h"
#include "typedefs.h"

#define E172_VARIANT_NUM_CONVERTER(NAME, TYPE) \
    inline TYPE to ## NAME(bool *ok = nullptr) const { return toNumber<TYPE>(ok); }

namespace e172 {

struct VariantBaseHandle { virtual ~VariantBaseHandle() {}; };
template<typename T>
struct VariantHandle : public VariantBaseHandle { T value; };

class VariantRTTIObject {
    template <typename T>
    friend class VariantRTTITable;
public:
    void destruct(VariantBaseHandle *h) const
    {
        if (m_destructor)
            m_destructor(h);
    }

    VariantBaseHandle *clone(VariantBaseHandle *h) const
    {
        if (m_copyConstructor)
            return m_copyConstructor(h);
        return nullptr;
    }

    std::string streamValue(VariantBaseHandle *h) const
    {
        if (m_streamValue)
            return m_streamValue(h);
        return "";
    }

    std::string toString(VariantBaseHandle *h) const
    {
        if (m_stringConvertor)
            return m_stringConvertor(h);
        return "";
    }

    bool compare(VariantBaseHandle *h0, VariantBaseHandle *h1) const
    {
        if (m_comparator)
            return m_comparator(h0, h1);
        return false;
    }

    bool less(VariantBaseHandle *h0, VariantBaseHandle *h1) const
    {
        if (m_lessOperator)
            return m_lessOperator(h0, h1);
        return false;
    }

    auto typeName() const { return m_typeName; }
    auto typeHash() const { return m_typeHash; }

private:
    template<typename T>
    VariantRTTIObject(TypeTag<T>) {
        m_destructor = [](VariantBaseHandle* obj) {
            delete dynamic_cast<VariantHandle<T>*>(obj);
        };
        m_copyConstructor = [](VariantBaseHandle* obj) {
            const auto casted_obj = dynamic_cast<VariantHandle<T>*>(obj);
            return new VariantHandle<T>(*casted_obj);
        };

        if constexpr(sfinae::StreamOperator<std::ostream, T>::value) {
            m_streamValue = [](VariantBaseHandle* obj) {
                VariantHandle<T>* casted_obj = dynamic_cast<VariantHandle<T>*>(obj);
                std::stringstream ss;
                    ss << casted_obj->value;
                return ss.str();
            };
        }

        if constexpr(std::is_same<T, std::string>::value || std::is_convertible<T, std::string>::value) {
            m_stringConvertor = [](VariantBaseHandle *obj) -> std::string {
                return dynamic_cast<VariantHandle<T>*>(obj)->value;
            };
        } else if constexpr (std::is_integral<T>::value || std::is_same<T, double>::value
                             || std::is_same<T, long double>::value
                             || std::is_same<T, float>::value) {
            m_stringConvertor = [](VariantBaseHandle* obj) {
                return std::to_string(dynamic_cast<VariantHandle<T>*>(obj)->value);
            };
        }

        if constexpr(sfinae::EquealOperator::exists<T>::value) {
            m_comparator = [](VariantBaseHandle* obj1, VariantBaseHandle* obj2) -> bool {
                return dynamic_cast<VariantHandle<T>*>(obj1)->value
                        == dynamic_cast<VariantHandle<T>*>(obj2)->value;
            };
        } else {
            m_comparator = nullptr;
        }

        if constexpr(sfinae::LessOperator::exists<T>::value) {
            m_lessOperator = [](VariantBaseHandle* obj1, VariantBaseHandle* obj2){
                return dynamic_cast<VariantHandle<T>*>(obj1)->value
                < dynamic_cast<VariantHandle<T>*>(obj2)->value;
            };
        }

        m_typeName = Type<T>::name();
        m_typeHash = Type<T>::hash();
    }

private:
    std::string m_typeName;
    size_t m_typeHash = 0;

    std::function<void(VariantBaseHandle*)> m_destructor;
    std::function<VariantBaseHandle*(VariantBaseHandle*)> m_copyConstructor;
    std::function<std::string(VariantBaseHandle*)> m_streamValue;
    std::function<std::string(VariantBaseHandle*)> m_stringConvertor;

    std::function<bool(VariantBaseHandle*, VariantBaseHandle*)> m_comparator;
    std::function<bool(VariantBaseHandle*, VariantBaseHandle*)> m_lessOperator;
};

class VariantRTTIPtr {
    template <typename T>
    friend class VariantRTTITable;
public:
    VariantRTTIPtr() = default;

    operator bool() const { return m_obj; }
    bool operator!() const { return m_obj == nullptr; }
    auto operator->() const { return m_obj; }

    bool operator==(VariantRTTIPtr other) const
    {
        if (m_obj == other.m_obj) {
            return true;
        }
#if defined(__MINGW32__) || (defined(_MSC_FULL_VER) && !defined(__INTEL_COMPILER))
        if (m_obj && other.m_obj) {
            /// Additional comparison needed on mingw and msvc if two rtti objects created in different libraries
            return m_obj->typeHash() == other.m_obj->typeHash();
        }
#endif
        return false;
    }

private:
    VariantRTTIPtr(const VariantRTTIObject *obj)
        : m_obj(obj)
    {}

private:
    const VariantRTTIObject* m_obj = nullptr;
};

template <typename T>
class VariantRTTITable {
public:
    static VariantRTTIPtr object() { return s_object; }
private:
    inline static VariantRTTIPtr s_object = new VariantRTTIObject(TypeTag<T>{});
};

class Variant;

using VariantVector = std::vector<Variant>;
using VariantList = std::list<Variant>;
using VariantMap = std::map<std::string, Variant>;

std::ostream &operator<<(std::ostream &stream, const VariantVector &vector);
std::ostream &operator<<(std::ostream &stream, const VariantList &list);
std::ostream &operator<<(std::ostream &stream, const VariantMap &map);

class Variant {
    friend std::ostream &operator<<(std::ostream &stream, const Variant &arg);
    VariantBaseHandle *m_data = nullptr;

#ifdef E172_USE_VARIANT_RTTI_OBJECT
    VariantRTTIPtr m_rttiObject;
#else
    std::string m_typeName;
    size_t m_typeHash = 0;

    std::function<void(VariantBaseHandle*)> m_destructor;
    std::function<VariantBaseHandle*(VariantBaseHandle*)> m_copy_constructor;
    std::function<std::string(VariantBaseHandle*)> m_stream_value;
    std::function<std::string(VariantBaseHandle*)> m_string_convertor;

    std::function<bool(VariantBaseHandle*, VariantBaseHandle*)> m_comparator;
    std::function<bool(VariantBaseHandle*, VariantBaseHandle*)> m_less_operator;
#endif

    /**
     * @brief valueUnchecked - get value without any checks
     * @note if variant is invalid or not contains T then undefined behaviour.
     * @return value containing in variant
     */
    template<typename T>
    T valueUnchecked() const { return dynamic_cast<VariantHandle<T>*>(m_data)->value; }

    template<typename T>
    static std::string containerToJson(const T& container) {
        std::string result;
        result += "[";
        size_t i = 0;
        for (auto cc : container) {
            result += cc.toJson();
            if (i < container.size() - 1) {
                result += ", ";
            }
            ++i;
        }
        result += "]";
        return result;
    }

public:
    // Variant base functional
    Variant() = default;

#ifndef E172_DISABLE_VARIANT_ABSTRACT_CONSTRUCTOR
    template<typename T>
    Variant(T value) { assign(value); }
#endif

    Variant(const Variant &obj) { operator=(obj); }

    template<typename T>
    void operator=(const T& value) { assign(value); }

    void operator=(const Variant &obj) {
#ifdef E172_USE_VARIANT_RTTI_OBJECT
        m_rttiObject = obj.m_rttiObject;
        if (obj.m_data && obj.m_rttiObject)
            m_data = obj.m_rttiObject->clone(obj.m_data);
#else
        m_typeName = obj.m_typeName;
        m_typeHash = obj.m_typeHash;
        m_destructor = obj.m_destructor;
        m_copy_constructor = obj.m_copy_constructor;
        m_stream_value = obj.m_stream_value;
        m_comparator = obj.m_comparator;
        m_less_operator = obj.m_less_operator;
        m_string_convertor = obj.m_string_convertor;
        if (obj.m_data && obj.m_copy_constructor)
            m_data = obj.m_copy_constructor(obj.m_data);
#endif
    }

    ~Variant() {
#ifdef E172_USE_VARIANT_RTTI_OBJECT
        if (m_data && m_rttiObject) {
            m_rttiObject->destruct(m_data);
        }
#else
        if (m_data && m_destructor) {
            m_destructor(m_data);
        }
#endif
    }

    template<typename T>
    T value() const
    {
        if (!containsType<T>()) {
            throw std::runtime_error("Variant does not contain type: " + Type<T>::name());
        }
        if (m_data)
            return dynamic_cast<VariantHandle<T>*>(m_data)->value;
        return T();
    }

    template<typename T>
    T valueOr(const T &defaultValue) const
    {
        if (containsType<T>()) {
            return dynamic_cast<VariantHandle<T>*>(m_data)->value;
        }
        return defaultValue;
    }

    template<typename T, typename R>
    R fold(
        const std::function<R(const T &)> &onOk,
        const std::function<R()> &onNull = []() { return R(); }) const
    {
        if (containsType<T>()) {
            return onOk(value<T>());
        } else {
            return onNull();
        }
    }

    template<typename T>
    void fold(
        const std::function<void(const T &)> &onOk,
        const std::function<void()> &onNull = []() {}) const
    {
        if (containsType<T>()) {
            onOk(value<T>());
        } else {
            onNull();
        }
    }

#ifdef E172_USE_VARIANT_RTTI_OBJECT
    template<typename T>
    void assign(const T& value) {
        if (!m_data || m_rttiObject != VariantRTTITable<T>::object()) {
            if (m_data)
                m_rttiObject->destruct(m_data);

            m_data = new VariantHandle<T>();
            m_rttiObject = VariantRTTITable<T>::object();
        }
        dynamic_cast<VariantHandle<T>*>(m_data)->value = value;
    }
#else
    template<typename T>
    void assign(const T& value) {
        auto hash = Type<T>().hash();
        if (hash != m_typeHash) {
            if (m_data)
                m_destructor(m_data);

            // main operators
            m_data = new VariantHandle<T>();
            m_destructor = [](VariantBaseHandle* obj){
                delete dynamic_cast<VariantHandle<T>*>(obj);
            };
            m_copy_constructor = [](VariantBaseHandle* obj) {
                const auto casted_obj = dynamic_cast<VariantHandle<T>*>(obj);
                return new VariantHandle<T>(*casted_obj);
            };

            // additional operators
            if constexpr(sfinae::StreamOperator<std::ostream, T>::value) {
                m_stream_value = [](VariantBaseHandle* obj) {
                    VariantHandle<T>* casted_obj = dynamic_cast<VariantHandle<T>*>(obj);
                    std::stringstream ss;
                        ss << casted_obj->value;
                    return ss.str();
                };
            } else {
                m_stream_value = nullptr;
            }

            if constexpr(std::is_same<T, std::string>::value || sfinae::TypeConvertionOperator<T, std::string>::value) {
                m_string_convertor = [](VariantBaseHandle* obj) {
                    return dynamic_cast<VariantHandle<T>*>(obj)->value;
                };
            } else if constexpr(std::is_integral<T>::value) {
                m_string_convertor = [](VariantBaseHandle* obj) {
                    return std::to_string(dynamic_cast<VariantHandle<T>*>(obj)->value);
                };
            } else {
                m_string_convertor = nullptr;
            }

            if constexpr(sfinae::EquealOperator::exists<T>::value) {
                m_comparator = [](VariantBaseHandle* obj1, VariantBaseHandle* obj2) -> bool {
                    return dynamic_cast<VariantHandle<T>*>(obj1)->value
                            == dynamic_cast<VariantHandle<T>*>(obj2)->value;
                };
            } else {
                m_comparator = nullptr;
            }

            if constexpr(sfinae::LessOperator::exists<T>::value) {
                m_less_operator = [](VariantBaseHandle* obj1, VariantBaseHandle* obj2){
                    return dynamic_cast<VariantHandle<T>*>(obj1)->value
                    < dynamic_cast<VariantHandle<T>*>(obj2)->value;
                };
            } else {
                m_less_operator = nullptr;
            }

            m_typeName = Type<T>().name();
            m_typeHash = hash;
        }

        dynamic_cast<VariantHandle<T>*>(m_data)->value = value;
    }
#endif


#ifdef E172_USE_VARIANT_RTTI_OBJECT
    std::string typeName() const { return m_rttiObject ? m_rttiObject->typeName() : ""; }
    template<typename T>
    bool containsType() const { return m_rttiObject == VariantRTTITable<T>::object(); }
#else
    std::string typeName() const { return m_typeName; }
    template<typename T>
    bool containsType() const { return m_typeHash == Type<T>().hash(); }
#endif
    friend bool operator==(const Variant &varian0, const Variant &varian1);
    inline friend bool operator!=(const Variant &varian0, const Variant &varian1) { return !(varian0 == varian1); };
    friend bool operator<(const Variant &varian0, const Variant &varian1);

    static bool typeSafeCompare(const Variant &varian0, const Variant &varian1);

    template<typename T>
    static Variant fromValue(const T &value) { Variant v; v.assign(value); return v; }


    // Static tools

    static bool containsNumber(const std::string& string);


    // User interface methods
    Variant(const std::string &value) { assign(value); }
    Variant(const char *value) : Variant(std::string(value)) {}
    Variant(const VariantMap &value) { assign(value); }
    Variant(const VariantList &value) { assign(value); }
    Variant(const VariantVector &value) { assign(value); }

    template<typename T>
    Variant(const Vector<T> &value)
    {
        assign(value);
    }

    Variant(double value) { assign(value); }

    Variant(std::uint8_t value) { assign(value); }
    Variant(std::uint16_t value) { assign(value); }
    Variant(std::uint32_t value) { assign(value); }
    Variant(std::uint64_t value) { assign(value); }

    Variant(std::int8_t value) { assign(value); }
    Variant(std::int16_t value) { assign(value); }
    Variant(std::int32_t value) { assign(value); }
    Variant(std::int64_t value) { assign(value); }

    VariantVector constrained() const;

    bool isNumber() const;
    bool isString() const;

#ifdef E172_USE_VARIANT_RTTI_OBJECT
    inline bool isNull() const { return !m_rttiObject; }
#else
    inline bool isNull() const { return m_typeName.size() <= 0; }
#endif

    template<typename T>
    T toNumber(bool *ok = nullptr) const;

    template<typename T>
    static constexpr bool isNumberType();

    E172_VARIANT_NUM_CONVERTER(Double, double)
    E172_VARIANT_NUM_CONVERTER(LongDouble, long double)
    E172_VARIANT_NUM_CONVERTER(Float, float)
    E172_VARIANT_NUM_CONVERTER(Int, int)
    E172_VARIANT_NUM_CONVERTER(UInt, unsigned int)
    E172_VARIANT_NUM_CONVERTER(Char, char)
    E172_VARIANT_NUM_CONVERTER(Bool, bool)

    E172_VARIANT_NUM_CONVERTER(UInt8, uint8_t)
    E172_VARIANT_NUM_CONVERTER(UInt16, uint16_t)
    E172_VARIANT_NUM_CONVERTER(UInt32, uint32_t)
    E172_VARIANT_NUM_CONVERTER(UInt64, uint64_t)

    E172_VARIANT_NUM_CONVERTER(Int8, int8_t)
    E172_VARIANT_NUM_CONVERTER(Int16, int16_t)
    E172_VARIANT_NUM_CONVERTER(Int32, int32_t)
    E172_VARIANT_NUM_CONVERTER(Int64, int64_t)

    E172_VARIANT_NUM_CONVERTER(Size_t, size_t)

    auto toMap() const { return valueOr<VariantMap>({}); };

    auto toList() const
    {
        if (containsType<VariantVector>()) {
            const auto l = valueOr<VariantVector>({});
            return VariantList(l.begin(), l.end());
        }
        return valueOr<VariantList>({});
    };

    auto toVector() const
    {
        if (containsType<VariantList>()) {
            const auto l = valueOr<VariantList>({});
            return VariantVector(l.begin(), l.end());
        }
        return valueOr<VariantVector>({});
    };

    template<typename T>
    auto toMathVector() const
    {
        return valueOr<Vector<T>>({});
    };

    std::string toString() const;
    static Variant fromString(const std::string &string);
    static VariantMap fromString(const std::map<std::string, std::string> &map);

    std::string toJson() const;
    static Variant fromJson(const std::string &json);
};

template<typename T>
constexpr bool Variant::isNumberType()
{
    if (std::is_same<T, bool>::value || std::is_same<T, char>::value
        || std::is_same<T, signed char>::value || std::is_same<T, unsigned char>::value
        || std::is_same<T, wchar_t>::value || std::is_same<T, char16_t>::value
        || std::is_same<T, char32_t>::value || std::is_same<T, short>::value              // NOLINT
        || std::is_same<T, unsigned short>::value || std::is_same<T, unsigned int>::value // NOLINT
        || std::is_same<T, unsigned long>::value || std::is_same<T, long long>::value     // NOLINT
        || std::is_same<T, unsigned long long>::value || std::is_same<T, float>::value    // NOLINT
        || std::is_same<T, double>::value || std::is_same<T, long double>::value) {
        return true;
    }
    return false;
}

template<typename T>
T Variant::toNumber(bool *ok) const
{
    if (ok)
        *ok = true;

    if (containsType<bool>()) {
        return static_cast<T>(valueUnchecked<bool>());
    } else if (containsType<char>()) {
        return static_cast<T>(valueUnchecked<char>());
    } else if (containsType<unsigned char>()) {
        return static_cast<T>(valueUnchecked<unsigned char>());
#ifdef _WCHAR_T_DEFINED
    } else if (containsType<wchar_t>()) {
        return static_cast<T>(valueUnchecked<wchar_t>());
#endif
#ifdef __CHAR16_TYPE__
    } else if (containsType<char16_t>()) {
        return static_cast<T>(valueUnchecked<char16_t>());
#endif
#ifdef __CHAR32_TYPE__
    } else if (containsType<char32_t>()) {
        return static_cast<T>(valueUnchecked<char32_t>());
#endif
    } else if (containsType<short>()) {                          // NOLINT
        return static_cast<T>(valueUnchecked<short>());          // NOLINT
    } else if (containsType<unsigned short>()) {                 // NOLINT
        return static_cast<T>(valueUnchecked<unsigned short>()); // NOLINT
    } else if (containsType<int>()) {
        return static_cast<T>(valueUnchecked<int>());
    } else if (containsType<unsigned int>()) {
        return static_cast<T>(valueUnchecked<unsigned int>());
    } else if (containsType<long>()) {                               // NOLINT
        return static_cast<T>(valueUnchecked<long>());               // NOLINT
    } else if (containsType<unsigned long>()) {                      // NOLINT
        return static_cast<T>(valueUnchecked<unsigned long>());      // NOLINT
    } else if (containsType<long long>()) {                          // NOLINT
        return static_cast<T>(valueUnchecked<long long>());          // NOLINT
    } else if (containsType<unsigned long long>()) {                 // NOLINT
        return static_cast<T>(valueUnchecked<unsigned long long>()); // NOLINT
    } else if (containsType<float>()) {
        return static_cast<T>(valueUnchecked<float>());
    } else if (containsType<double>()) {
        return static_cast<T>(valueUnchecked<double>());
    } else if (containsType<long double>()) {
        return static_cast<T>(valueUnchecked<long double>());
    } else if (containsType<std::string>()) {
        try {
            if constexpr (std::is_same<T, double>::value) {
                return std::stod(valueUnchecked<std::string>());
            } else if constexpr (std::is_same<T, float>::value) {
                return std::stof(valueUnchecked<std::string>());
            } else if constexpr (std::is_signed<T>::value) {
                return static_cast<T>(std::stoll(valueUnchecked<std::string>()));
            } else {
                return static_cast<T>(std::stoull(valueUnchecked<std::string>()));
            }
        } catch (std::invalid_argument) {
            if (ok)
                *ok = false;
            return 0;
        }
    }

    if (ok)
        *ok = false;
    return 0;
}

} // namespace e172
