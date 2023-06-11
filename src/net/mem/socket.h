#pragma once

#include <memory>
#include <queue>
#include <src/net/socket.h>
#include <src/utility/ringbuf.h>

namespace e172 {

class MemSocket : public Socket
{
public:
    class Channel
    {
    public:
        const std::shared_ptr<std::queue<Byte>> &writeQueue() const { return m_writeQueue; };
        const std::shared_ptr<std::queue<Byte>> &readQueue() const { return m_readQueue; };
        bool inUse() const { return m_writeQueue.use_count() > 1 && m_readQueue.use_count() > 1; }

        Channel inverted() { return Channel(m_readQueue, m_writeQueue); }

        static Channel make()
        {
            return Channel(std::make_shared<std::queue<Byte>>(),
                           std::make_shared<std::queue<Byte>>());
        }

    private:
        Channel(const std::shared_ptr<std::queue<Byte>> &writeQueue,
                const std::shared_ptr<std::queue<Byte>> &readQueue)
            : m_writeQueue(writeQueue)
            , m_readQueue(readQueue)
        {}

    private:
        std::shared_ptr<std::queue<Byte>> m_writeQueue;
        std::shared_ptr<std::queue<Byte>> m_readQueue;
    };

public:
    MemSocket(const Channel &channel)
        : m_channel(channel)
    {}

    // Write interface
public:
    std::size_t write(const Byte *bytes, std::size_t size) override
    {
        std::lock_guard lock(m_mutex);
        if (m_channel.inUse()) {
            for (std::size_t i = 0; i < size; ++i) {
                m_channel.writeQueue()->push(bytes[i]);
            }
            return size;
        }
        return 0;
    }
    void flush() override {}

    // Read interface
public:
    std::size_t bufferize() override
    {
        std::lock_guard lock(m_mutex);
        if (m_channel.inUse()) {
            std::size_t i = 0;
            while (!m_channel.readQueue()->empty() && !m_buf.is_full()) {
                if (m_buf.push(m_channel.readQueue()->front())) {
                    m_channel.readQueue()->pop();
                    ++i;
                }
            }
            return i;
        }
        return 0;
    }

    std::size_t bytesAvailable() const override { return m_buf.len(); }

    std::size_t read(Byte *dst, std::size_t size) override
    {
        const auto c = std::min(m_buf.len(), size);
        for (std::size_t i = 0; i < c; ++i) {
            dst[i] = m_buf.pop().value();
        }
        return c;
    }

    std::size_t peek(Byte *dst, std::size_t size) const override { return m_buf.peek(dst, size); }

    // Socket interface
public:
    bool isConnected() const override
    {
        std::lock_guard lock(m_mutex);
        return m_channel.inUse();
    }

private:
    Channel m_channel;
    mutable std::mutex m_mutex;
    RingBuf<Byte, 4096> m_buf;
};

} // namespace e172
