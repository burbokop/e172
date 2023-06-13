#pragma once

#include <functional>
#include <optional>
#include <ostream>
#include <sstream>
#include <variant>

namespace e172 {

template<typename L>
class EitherUnwrapException : public std::exception
{
public:
    EitherUnwrapException(const L &val)
        : m_message(std::string("Expected Right but got Left(") + stringify(val) + ")"){};

    // exception interface
public:
    const char *what() const noexcept override { return m_message.c_str(); }

private:
    static std::string stringify(const L &val)
    {
        std::ostringstream ss;
        ss << val;
        return ss.str();
    }

private:
    std::string m_message;
};

template<typename L>
class Left;
template<typename R>
class Right;

template<typename L, typename R>
class Either
{
    template<typename T>
    friend class Left;
    template<typename T>
    friend class Right;

public:
    typedef L left_type;
    typedef R right_type;

    static Either<L, R> fromStdVariant(const std::variant<L, R> &var) { return var; }

    operator bool() const { return m_data.index(); }
    bool defined() const { return m_data.index(); }

    inline auto left() const { return operator bool() ? Left<L>() : Left(leftValue()); }
    inline auto right() const { return operator bool() ? Right(rightValue()) : Right<R>(); }

    auto unwrap() const
    {
        if (*this) {
            return rightValue();
        } else {
            throw EitherUnwrapException(leftValue());
        }
    }

    template<typename Result>
    Result fold(const std::function<Result(const L &)> &onLeft,
                const std::function<Result(const R &)> &onRight) const
    {
        if (*this) {
            return onRight(right());
        } else {
            return onLeft(left());
        }
    }

    void fold(const std::function<void(const L &)> &onLeft,
              const std::function<void(const R &)> &onRight) const
    {
        if (*this) {
            onRight(right());
        } else {
            onLeft(left());
        }
    }

    template<typename NR>
    Either<L, NR> map(const std::function<NR(const R &)> &f)
    {
        if (*this) {
            return Either<L, NR>::fromStdVariant(
                std::variant<L, NR>(std::in_place_type<NR>, f(rightValue())));
        }
        return Either<L, NR>::fromStdVariant(
            std::variant<L, NR>(std::in_place_type<L>, leftValue()));
    }

    template<typename NR>
    Either<L, NR> flatMap(const std::function<Either<L, NR>(const R &)> &f)
    {
        if (*this) {
            return f(right());
        }
        return *this;
    }

    template<typename NL>
    Either<NL, R> mapLeft(const std::function<NL(const L &)> &f)
    {
        if (!*this) {
            return f(left());
        }
        return *this;
    }

    template<typename NL>
    Either<NL, R> flatMapLeft(const std::function<Either<NL, R>(const L &)> &f)
    {
        if (!*this) {
            return f(left());
        }
        return *this;
    }

    std::optional<R> option()
    {
        if (*this) {
            return rightValue();
        } else {
            return std::nullopt;
        }
    }

    R getOr(const R &defaultValue = {})
    {
        if (*this) {
            return rightValue();
        } else {
            return defaultValue;
        }
    }

    bool operator==(const Either<L, R> &other) const { return this->m_data == other.m_data; }
    bool operator<(const Either<L, R> &other) const { return this->m_data < other.m_data; }

private:
    Either(const std::variant<L, R> &var)
        : m_data(var)
    {}

    L leftValue() const { return std::get<0>(m_data); }
    R rightValue() const { return std::get<1>(m_data); }

private:
    std::variant<L, R> m_data;
};

template<typename L>
class Left
{
    template<typename EL, typename ER>
    friend class Either;
    std::optional<L> m_value;
    Left() {}

public:
    typedef L left_type;

    Left(const L &value) { m_value = value; }

    template<typename R>
    operator Either<L, R>() const
    {
        return std::variant<L, R>(std::in_place_type<L>, *m_value);
    }
    operator bool() const { return m_value.has_value(); }
    const L value() const { return *m_value; };

    bool operator==(const Left<L> &other) const { return this->m_value == other.m_value; }
};

template<typename R>
class Right
{
    template<typename EL, typename ER>
    friend class Either;
    std::optional<R> m_value;
    Right() {}

public:
    typedef R right_type;

    Right(const R &value) { m_value = value; }

    template<typename L>
    operator Either<L, R>() const
    {
        return std::variant<L, R>(std::in_place_type<R>, *m_value);
    }
    operator bool() const { return m_value.has_value(); }
    const R value() const { return m_value.value(); };

    bool operator==(const Right<R> &other) const { return this->m_value == other.m_value; }
};

/*
WORKING ON GCC BUT NOT ON MSVC
template<template<typename> typename C, typename L, typename R>
either<C<L>, C<R>> partition(const C<either<L, R>>& collection) {
    C<L> left;
    C<R> right;
    for(const auto& item : collection) {
        if(item) {
            right.push_back(item.right_value());
        } else {
            left.push_back(item.left_value());
        }
    }
    if(left.size() > 0) {
        return wall_e::left(left);
    } else {
        return wall_e::right(right);
    }
}*/

template<template<typename, typename> typename C, typename L, typename R>
Either<C<L, std::allocator<L>>, C<R, std::allocator<R>>> partition(
    const C<Either<L, R>, std::allocator<Either<L, R>>> &collection)
{
    C<L, std::allocator<L>> left;
    C<R, std::allocator<R>> right;
    for (const auto &item : collection) {
        if (item) {
            right.push_back(item.right_value());
        } else {
            left.push_back(item.left_value());
        }
    }
    if (left.size() > 0) {
        return Left(left);
    } else {
        return Right(right);
    }
}

template<typename L, typename R>
std::ostream &operator<<(std::ostream &stream, const Either<L, R> &e)
{
    if (e) {
        return stream << "Right(" << e.right_value() << ")";
    } else {
        return stream << "Left(" << e.left_value() << ")";
    }
}

} // namespace e172
