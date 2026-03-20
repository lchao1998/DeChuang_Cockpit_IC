#ifndef AUTOSOCM_APPLICATION_HPP
#define AUTOSOCM_APPLICATION_HPP

#include "client.hpp"
#include "service.hpp"
#include <memory>

namespace AutoSOCM {

class Application
{
public:
    virtual ~Application(){}

    virtual std::string getName() = 0;

    virtual std::shared_ptr<Client> createClient(uint16_t service, uint16_t instance) = 0;

    virtual std::shared_ptr<Service> createService(uint16_t service, uint16_t instance) = 0;

    virtual void start() = 0;

    virtual void startAsync() = 0;

    virtual void stop() = 0;

    virtual bool isRunning() = 0;
};

extern std::shared_ptr<Application> getApplication();

}
#endif
