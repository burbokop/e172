#include "bullet.h"
#include "shooter.h"
#include "src/net/impl/linux/networkerimpl.h"
#include <signal.h>
#include <src/gameapplication.h>
#include <src/testing.h>
#include <src/utility/defer.h>

namespace e172::integration {

constexpr std::uint16_t port = 2363;

class MocEventProvider : public AbstractEventProvider
{
public:
    MocEventProvider() = default;
    void pushEvent(const Event &event) { m_queue.push(event); }

    // AbstractEventProvider interface
public:
    std::optional<Event> pullEvent() override
    {
        if (m_queue.empty()) {
            return std::nullopt;
        } else {
            const auto e = m_queue.front();
            m_queue.pop();
            return e;
        }
    }

private:
    std::queue<Event> m_queue;
};

int withApplication(const std::function<void(GameApplication &app, e172::Networker &)> &f)
{
    GameApplication app(0, nullptr);
    app.setRenderInterval(1000 / 30);

    std::unique_ptr<e172::Networker> net = std::make_unique<e172::LinuxNetworkerImpl>();

    net->registerEntityType<Shooter>(0);
    net->registerEntityType<Bullet>();

    f(app, *net);
    return app.exec();
}

int serverMain()
{
    return withApplication([](GameApplication &app, Networker &net) {
        auto server = net.listen(app, port).unwrap();
        app.setMode(e172::GameApplication::Mode::Proceed);
        app.setEventProvider(server);
        app.scheduleRepeated(1000 / 30, [server] { server->sync(); });

        server->clientConnected().connect([&app](e172::PackedClientId id) {
            app.addEntity(e172::FactoryMeta::make<Shooter>(id));
        });
    });
}

class ClientServerSpec
{
    static void test0() e172_test(ClientServerSpec, test0);
};

void ClientServerSpec::test0()
{
    std::size_t shots = 0;
    const auto result = withApplication([&shots](GameApplication &app, Networker &net) {
        auto client = net.connect(app, port).unwrap();
        app.setMode(e172::GameApplication::Mode::Render);
        auto eventProvider = std::make_shared<MocEventProvider>();
        app.setEventProvider(eventProvider);
        app.scheduleRepeated(1000 / 30, [client] { client->sync(); });

        eventProvider->pushEvent(Event::mouseMotion({10, 10}));
        app.scheduleRepeated(100, [&shots, &app, eventProvider] {
            if (shots++ > 50) {
                app.quitLater();
            } else {
                eventProvider->pushEvent(Event::keyDown(ScancodeSpace));
                eventProvider->pushEvent(Event::keyUp(ScancodeSpace));
            }
        });
    });
    e172_shouldEqual(result, 0);
}

} // namespace e172::integration

template<typename... Args>
int execProcess(const std::string &path, Args... args)
{
    /// TODO make platform independent
    const auto pid = fork();
    if (pid < 0)
        throw std::runtime_error("Failed to fork");
    if (!pid && execl(path.c_str(), args..., nullptr)) {
        throw std::runtime_error("execl failed");
    }
    return pid;
}

int main(int argc, const char **argv)
{
    e172::Debug::installHandler([](const std::string &msg, e172::Debug::MessageType type) {
        if (type == e172::Debug::WarningMessage || type == e172::Debug::FatalMessage) {
            throw std::runtime_error(msg);
        }
    });

    if (argc == 2 && std::string(argv[1]) == "-s") {
        return e172::integration::serverMain();
    } else if (argc > 0) {
        const auto pid = execProcess(argv[0], argv[0], "-s");
        sleep(3);
        e172::Defer closePid([pid]() { kill(pid, SIGABRT); });
        return e172::testing::exec(argc, argv);
    } else {
        throw std::runtime_error("args empty");
    }
}
