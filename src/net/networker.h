#pragma once

#include "server.h"
#include <list>
#include <src/entity.h>
#include <src/todo.h>
#include <src/utility/either.h>
#include <vector>

namespace e172 {

template<typename T>
concept Serialize = requires(T const v)
{
    {
        v.serialize()
    } -> std::convertible_to<std::vector<std::uint8_t>>;
}
|| std::is_fundamental<T>::value;

template<typename T>
concept Deserialize = requires(std::vector<std::uint8_t> input)
{
    {
        T::deserialize(input)
    } -> std::convertible_to<T>;
}
|| std::is_fundamental<T>::value;

class AbstractNetSync
{
    friend Entity;

public:
    AbstractNetSync() = default;
    ~AbstractNetSync() = default;

    virtual bool dirty() const = 0;

protected:
    virtual std::vector<std::uint8_t> serialize() const = 0;
    virtual void deserAssign(const std::vector<std::uint8_t> &) = 0;
};

class GameApplication;

class GameClient
{
    friend class Networker;
    struct Private
    {};

public:
    GameClient(GameApplication &app, const std::shared_ptr<Socket> &socket, Private)
        : m_app(app)
        , m_socket(socket)
    {}

    void sync();

private:
    GameApplication &m_app;
    std::shared_ptr<Socket> m_socket;
};

class GameServer
{
    friend class Networker;
    struct Private
    {};

public:
    GameServer(GameApplication &app, const std::shared_ptr<Server> &server, Private)
        : m_app(app)
        , m_server(server)
    {}

    void sync();

private:
    GameApplication &m_app;
    std::shared_ptr<Server> m_server;
    std::list<std::shared_ptr<Socket>> m_sockets;
};

class Networker
{
public:
    enum Error {
        FailedToCreateSocket,
        ConnectionRefused,
        UnwnownConnectionError,
        BindingFailed,
        ListenFailed
    };

    static constexpr const char *localhost = "127.0.0.1";

    Networker() = default;

    friend std::ostream &operator<<(std::ostream &stream, Error e)
    {
        switch (e) {
        case Networker::FailedToCreateSocket:
            return stream << "FailedToCreateSocket";
        case Networker::ConnectionRefused:
            return stream << "ConnectionRefused";
        case Networker::UnwnownConnectionError:
            return stream << "UnwnownConnectionError";
        case Networker::BindingFailed:
            return stream << "BindingFailed";
        case Networker::ListenFailed:
            return stream << "ListenFailed";
        }
        return stream << "UnknownError";
    }

    Either<Error, std::shared_ptr<GameServer>> listen(GameApplication &app, std::uint16_t port)
    {
        return listen(port).map<std::shared_ptr<GameServer>>(
            [&app](const std::shared_ptr<Server> &s) {
                return std::make_shared<GameServer>(app, s, GameServer::Private{});
            });
    }

    Either<Error, std::shared_ptr<GameClient>> connect(GameApplication &app,
                                                       std::uint16_t port,
                                                       const std::string &address = localhost)
    {
        return connect(port, address).map<std::shared_ptr<GameClient>>([&app](auto s) {
            return std::make_shared<GameClient>(app, s, GameClient::Private{});
        });
    }

    virtual Either<Error, std::shared_ptr<Server>> listen(std::uint16_t port) = 0;
    virtual Either<Error, std::shared_ptr<Socket>> connect(std::uint16_t port,
                                                           const std::string &address = localhost)
        = 0;

    virtual ~Networker() = default;
};

// clang-format off
template<typename T>
requires Serialize<T> && Deserialize<T>
class NetSync : public AbstractNetSync
// clang-format on
{
public:
    NetSync(const T &val, Entity &e)
        : m_value(val)
    {
        e.installNetSync(this);
    }

    NetSync<T> &operator=(const T &val)
    {
        m_value = val;
        m_dirty = true;
        return *this;
    }

    operator T() const { return m_value; }

    // AbstractNetSync interface
public:
    bool dirty() const override { return m_dirty; }

protected:
    std::vector<uint8_t> serialize() const override
    {
        if constexpr (std::is_fundamental<T>::value) {
            return WriteBuffer::toBytes(m_value);
        } else {
            return m_value.serialize();
        }
    }

    void deserAssign(const std::vector<uint8_t> &b) override
    {
        if constexpr (std::is_fundamental<T>::value) {
            m_value = ReadBuffer::fromBytes<T>(b);
        } else {
            m_value = T::deserialize(b);
        }
    }

private:
    T m_value;
    bool m_dirty = false;
};

} // namespace e172
