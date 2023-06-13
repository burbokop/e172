#include "ringbufspec.h"

#include <iomanip>
#include <queue>
#include <src/time/elapsedtimer.h>
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

namespace {
template<std::size_t C>
void streamTestWithCapacity()
{
    constexpr std::size_t count = 1024 * 256;

    std::queue<std::uint8_t> dataToSend;
    for (std::size_t i = 0; i < count; ++i) {
        const std::uint32_t d = static_cast<std::uint32_t>(i);
        const std::uint8_t *ptr = reinterpret_cast<const std::uint8_t *>(&d);
        for (std::size_t j = 0; j < sizeof(std::uint32_t); ++j) {
            dataToSend.push(ptr[j]);
        }
    }
    std::vector<std::uint32_t> receivedData;

    RingBuf<std::uint8_t, C> buf;

    ElapsedTimer timeout = 4000;

    std::uint32_t nextData = 0;
    while (receivedData.size() < count) {
        if (!dataToSend.empty()) {
            if (buf.push(dataToSend.front())) {
                dataToSend.pop();
            }
        }

        {
            std::vector<std::uint8_t> b;
            b.resize(sizeof(std::uint32_t));
            if (buf.peek(b.data(), b.size()) == sizeof(std::uint32_t)) {
                e172_shouldEqual(*reinterpret_cast<uint32_t *>(b.data()), nextData);
            }
        }

        if (buf.len() >= sizeof(std::uint32_t)) {
            std::vector<std::uint8_t> b;
            b.resize(sizeof(std::uint32_t));
            for (std::size_t i = 0; i < sizeof(std::uint32_t); ++i) {
                b[i] = buf.pop().value();
            }
            const auto data = *reinterpret_cast<uint32_t *>(b.data());
            e172_shouldEqual(data, nextData);
            receivedData.push_back(data);
            ++nextData;
        }
        if(timeout.check()) {
            throw std::runtime_error("data transmission timeout");
        };
    }

    e172_shouldEqual(receivedData.size(), count);
    for (std::size_t i = 0; i < receivedData.size(); ++i) {
        e172_shouldEqual(receivedData[i], i);
    }
}
} // namespace

void RingBufSpec::streamTestWithCapacity5()
{
    streamTestWithCapacity<5>();
}

void RingBufSpec::streamTestWithCapacity6()
{
    streamTestWithCapacity<6>();
}

void RingBufSpec::streamTestWithCapacity7()
{
    streamTestWithCapacity<7>();
}

void RingBufSpec::streamTestWithCapacity8()
{
    streamTestWithCapacity<8>();
}

void RingBufSpec::streamTestWithCapacity9()
{
    streamTestWithCapacity<9>();
}

void RingBufSpec::streamTestWithCapacity15()
{
    streamTestWithCapacity<15>();
}

void RingBufSpec::streamTestWithCapacity16()
{
    streamTestWithCapacity<16>();
}

void RingBufSpec::streamTestWithCapacity111()
{
    streamTestWithCapacity<111>();
}

} // namespace e172::tests
