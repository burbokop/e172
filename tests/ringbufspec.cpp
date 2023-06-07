#include "ringbufspec.h"

#include <iomanip>
#include <src/utility/ringbuf.h>

namespace std {

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::optional<T> &o)
{
    if (o) {
        return stream << *o;
    } else {
        return stream << "std::nullopt";
    }
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, std::nullopt_t)
{
    return stream << "std::nullopt";
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &bytes)
{
    std::size_t i = 0;
    for (const auto &b : bytes) {
        stream << std::hex << std::setfill('0') << std::setw(2 * sizeof(T)) << int(b);
        if (i++ < bytes.size() - 1) {
            stream << '.';
        }
    }
    return stream;
}

} // namespace std

namespace e172::tests {

void RingBufSpec::untilFullTest()
{
    RingBuf<std::uint8_t, 4> buf;

    e172_shouldEqual(buf.len(), 0);
    e172_shouldEqual(buf.push_ability(), 3);
    e172_shouldEqual(buf.is_empty(), true);
    e172_shouldEqual(buf.is_full(), false);

    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.len(), 1);
    e172_shouldEqual(buf.push_ability(), 2);
    e172_shouldEqual(buf.is_empty(), false);
    e172_shouldEqual(buf.is_full(), false);

    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.len(), 2);
    e172_shouldEqual(buf.push_ability(), 1);
    e172_shouldEqual(buf.is_empty(), false);
    e172_shouldEqual(buf.is_full(), false);

    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.len(), 3);
    e172_shouldEqual(buf.push_ability(), 0);
    e172_shouldEqual(buf.is_empty(), false);
    e172_shouldEqual(buf.is_full(), true);

    e172_shouldEqual(buf.push(0), false);
    e172_shouldEqual(buf.len(), 3);
    e172_shouldEqual(buf.push_ability(), 0);
    e172_shouldEqual(buf.is_empty(), false);
    e172_shouldEqual(buf.is_full(), true);
}

void RingBufSpec::pushPopTest()
{
    RingBuf<std::uint8_t, 4> buf;
    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.push(1), true);
    e172_shouldEqual(buf.push_ability(), 1);

    e172_shouldEqual(buf.pop().value(), 0);
    e172_shouldEqual(buf.pop().value(), 1);
    e172_shouldEqual(buf.push_ability(), 3);

    e172_shouldEqual(buf.push(2), true);
    e172_shouldEqual(buf.push(3), true);
    e172_shouldEqual(buf.push_ability(), 1);

    e172_shouldEqual(buf.pop().value(), 2);
    e172_shouldEqual(buf.pop().value(), 3);
    e172_shouldEqual(buf.push_ability(), 3);

    e172_shouldEqual(buf.push(4), true);
    e172_shouldEqual(buf.push(5), true);
    e172_shouldEqual(buf.push_ability(), 1);

    e172_shouldEqual(buf.push(6), true);
    e172_shouldEqual(buf.push(7), false);
    e172_shouldEqual(buf.push_ability(), 0);

    e172_shouldEqual(buf.pop().value(), 4);
    e172_shouldEqual(buf.pop().value(), 5);
    e172_shouldEqual(buf.push_ability(), 2);

    e172_shouldEqual(buf.push(8), true);
    e172_shouldEqual(buf.push(9), true);

    e172_shouldEqual(buf.push_ability(), 0);

    e172_shouldEqual(buf.pop().value(), 6);
    e172_shouldEqual(buf.pop().value(), 8);
    e172_shouldEqual(buf.pop().value(), 9);

    e172_shouldEqual(buf.pop(), std::optional<std::uint8_t>());
    e172_shouldEqual(buf.push_ability(), 3);
}

void RingBufSpec::topTest()
{
    RingBuf<std::uint8_t, 4> buf;
    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.push(1), true);
    e172_shouldEqual(buf.len(), 2);

    e172_shouldEqual(buf.top().value(), 0);
    e172_shouldEqual(buf.len(), 2);

    e172_shouldEqual(buf.pop().value(), 0);
    e172_shouldEqual(buf.len(), 1);
    e172_shouldEqual(buf.pop().value(), 1);
    e172_shouldEqual(buf.len(), 0);
    e172_shouldEqual(buf.top(), std::optional<std::uint8_t>());
}

void RingBufSpec::peekTest()
{
    RingBuf<std::uint8_t, 4> buf;

    e172_shouldEqual(buf.push(0), true);
    e172_shouldEqual(buf.push(1), true);
    e172_shouldEqual(buf.len(), 2);

    std::vector<std::uint8_t> tmp(2, 0);
    e172_shouldEqual(buf.peek(tmp.data(), tmp.size()), 2);
    e172_shouldEqual(tmp, e172_initializerList(std::vector<std::uint8_t>, 0, 1));
}

void RingBufSpec::peekNotAllTest()
{
    RingBuf<std::uint8_t, 4> buf;

    e172_shouldEqual(buf.push(1), true);
    e172_shouldEqual(buf.push(2), true);
    e172_shouldEqual(buf.len(), 2);

    std::vector<std::uint8_t> tmp(4, 0);
    e172_shouldEqual(buf.peek(tmp.data(), tmp.size()), 2);
    e172_shouldEqual(tmp, e172_initializerList(std::vector<std::uint8_t>, 1, 2, 0, 0));
}

} // namespace e172::tests
