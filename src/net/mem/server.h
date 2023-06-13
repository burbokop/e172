#pragma once

#include "socket.h"
#include <src/net/server.h>

namespace e172 {

/**
 * @brief The MemServer class implements communucation through internal application memory
 * Can be used for tests or to simulate some real communication
 */
class MemServer : public Server
{
public:
    MemServer() = default;

    void pushConnection(const std::shared_ptr<MemSocket> &s)
    {
        std::lock_guard lock(m_mutex);
        m_connectionQueue.push(s);
    }

    // Server interface
public:
    std::shared_ptr<Socket> pullConnection() override
    {
        std::lock_guard lock(m_mutex);
        if (m_connectionQueue.empty()) {
            return nullptr;
        } else {
            const auto s = m_connectionQueue.front();
            m_connectionQueue.pop();
            return s;
        }
    }

private:
    std::queue<std::shared_ptr<MemSocket>> m_connectionQueue;
    std::mutex m_mutex;
};

} // namespace e172
