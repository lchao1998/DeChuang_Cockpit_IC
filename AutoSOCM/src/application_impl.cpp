#include "application_impl.hpp"
#include "client_impl.hpp"
#include "service_impl.hpp"
#include "protocol/runtime.hpp"
#include "protocol/application.hpp"
#include <iostream>

#define APP_NAME_ENV "AUTOSOCM_APP_NAME"
#define DEFAULT_APP_NAME "AutoSOCM Application"

namespace AutoSOCM
{

std::shared_ptr<Application> getApplication()
{
    static const char* appNameEnv = getenv(APP_NAME_ENV);
    static std::string appName = appNameEnv ? appNameEnv : DEFAULT_APP_NAME;
    static std::shared_ptr<Application> app = std::make_shared<ApplicationImpl>(appName);
    return app;
}

ApplicationImpl::ApplicationImpl(const std::string &name):
    m_appName(name),
    m_protocolApp(Protocol::Runtime::get()->createApplication(name)),
    m_running(false)
{
}

ApplicationImpl::~ApplicationImpl()
{
    if(m_running)
        stop();
}

std::string ApplicationImpl::getName()
{
    return m_protocolApp->getName();
}

std::shared_ptr<Client> ApplicationImpl::createClient(uint16_t service, uint16_t instance)
{
    std::shared_ptr<ClientImpl> clientImpl = std::make_shared<ClientImpl>(service, instance, m_appName);
    return clientImpl;
}

std::shared_ptr<Service> ApplicationImpl::createService(uint16_t service, uint16_t instance)
{
    std::shared_ptr<ServiceImpl> serviceImpl = std::make_shared<ServiceImpl>(service, instance, m_appName);
    return serviceImpl;
}

void ApplicationImpl::start()
{
    {
        std::lock_guard<std::mutex> locker(m_mtx);
        if(!m_running)
        {
            m_running = true;
            m_runningThreadID = std::this_thread::get_id();
        }
        else
            return;
    }
    m_protocolApp->start();
}

void ApplicationImpl::startAsync()
{
    std::lock_guard<std::mutex> locker(m_mtx);
    if(!m_running)
    {
        m_running = true;
        m_runningThreadID = std::this_thread::get_id();
    }
    else
        return;

    m_runThreadReturn = std::async ([&](){
        m_protocolApp->start();
    });
}

void ApplicationImpl::stop()
{
    std::lock_guard<std::mutex> locker(m_mtx);
    if(m_running)
    {
        m_running = false;
        m_protocolApp->stop();
        if(m_runThreadReturn.valid())
            m_runThreadReturn.wait();
    }
}

bool ApplicationImpl::isRunning()
{
    std::unique_lock<std::mutex> locker(m_mtx);
    return m_running;
}


}
