#ifndef AUTOSOCM_APPLICATION_IMPL_HPP
#define AUTOSOCM_APPLICATION_IMPL_HPP

#include "application.hpp"
#include <mutex>
#include <future>

namespace Protocol {
class Application;
}

namespace AutoSOCM {
class ApplicationImpl: public Application
{
public:
    explicit ApplicationImpl(const std::string& name);

    ~ApplicationImpl();

    std::string getName();

    std::shared_ptr<Client> createClient(uint16_t service, uint16_t instance);

    std::shared_ptr<Service> createService(uint16_t service, uint16_t instance);

    void start();

    void startAsync();

    void stop();

    bool isRunning();

private:
    bool m_running;
    std::future<void> m_runThreadReturn;
    std::string m_appName;
    std::thread::id m_runningThreadID;
    std::mutex m_mtx;
    std::shared_ptr<Protocol::Application> m_protocolApp;
};
}
#endif
