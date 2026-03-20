#include "application.hpp"
#include "common.hpp"
#include <vsomeip/vsomeip.hpp>
#include <assert.h>

namespace Protocol {

Application::Application(const std::string &name):
    m_name(name),
    m_rtm(vsomeip::runtime::get()),
    m_app(m_rtm->create_application(name))
{
    assert(m_app->init());
    m_io.reset();
}

Application::~Application()
{
}

std::string Application::getName() const
{
    return m_name;
}

std::shared_ptr<Endpoint> Application::createEndpoint(int16_t service, int16_t instance, Endpoint::Type type)
{
    std::lock_guard<std::mutex> locker(m_mtx);

    std::shared_ptr<Endpoint> endpoint;
    RegisteredEndpointList& endpointList = m_registeredEndpointList[service][instance];

    if(!endpointList.service && endpointList.clientList.empty())
        m_app->register_message_handler(service, instance, defaultMethodID, std::bind(&Application::onMessageReceived,this, std::placeholders::_1));

    if(type == Endpoint::Type::Service)
    {
        assert(!endpointList.service);
        m_app->offer_event(service, instance, defaultEventID, {defaultEventGroupID}, false);
        m_app->offer_service(service,instance);
        endpoint = std::make_shared<Endpoint>(m_name, service, instance, type);
        endpointList.service = endpoint;
    }
    else
    {
        if(endpointList.clientList.empty())
        {
            m_app->request_event(service, instance, defaultEventID, {defaultEventGroupID}, false);
            m_app->subscribe(service, instance, defaultEventGroupID);
            m_app->request_service(service,instance);

            m_app->register_message_handler(service,
                                            instance,
                                            defaultEventID,
                                            std::bind(&Application::onMessageReceived,this, std::placeholders::_1));

            m_app->register_availability_handler(service,
                                                 instance,
                                                 std::bind(&Application::onAvailabilityChanged, this, std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
        }
        endpoint = std::make_shared<Endpoint>(m_name, service, instance, type);
        endpointList.clientList.push_back(endpoint);
    }
    return endpoint;
}

void Application::removeEndpoint(std::shared_ptr<Endpoint> endpoint)
{
    std::lock_guard<std::mutex> locker(m_mtx);

    uint16_t service = endpoint->getService();
    uint16_t instance = endpoint->getInstance();
    RegisteredEndpointList& endpointList = m_registeredEndpointList[service][instance];
    bool removed = false;

    if(endpoint == endpointList.service)
    {
        m_app->stop_offer_event(service, instance, defaultEventID);
        m_app->stop_offer_service(service, instance);
        endpointList.service = nullptr;
        removed = true;
    }
    else
    {
        EndpointList::iterator iter = endpointList.clientList.begin();
        for(; iter!=endpointList.clientList.end(); ++iter)
        {
            if(*iter == endpoint)
                break;
        }

        if(iter != endpointList.clientList.end())
        {
            endpointList.clientList.erase(iter);
            if(endpointList.clientList.empty())
            {
                m_app->unsubscribe(service, instance, defaultEventGroupID);
                m_app->release_event(service, instance, defaultEventID);
                m_app->release_service(service, instance);
                m_app->unregister_message_handler(service, instance, defaultEventID);
            }
            removed = true;
        }
    }

    if(removed && !endpointList.service  && endpointList.clientList.empty())
    {
        m_app->unregister_message_handler(service, instance, defaultMethodID);
    }
}

void Application::start()
{
    boost::asio::io_service::work work(m_io);
    m_runThread = std::thread([&](){
        m_app->start();
    });
    m_io.run();
}

void Application::stop()
{
    m_app->stop();
    m_io.stop();
    m_runThread.join();
}

void Application::onMessageReceived(const std::shared_ptr<vsomeip::message> &msg)
{
    m_io.post([&,msg](){
        vsomeip::service_t service = msg->get_service();
        vsomeip::instance_t instance = msg->get_instance();
        vsomeip::message_type_e msgType = msg->get_message_type();

        std::lock_guard<std::mutex> locker(m_mtx);
        RegisteredEndpointList& endpointList = m_registeredEndpointList[service][instance];

        if(msgType == vsomeip::message_type_e::MT_REQUEST && endpointList.service)
        {
            endpointList.service->onMessageReceived(msg);
        }
        else if(((msgType == vsomeip::message_type_e::MT_RESPONSE) || (msgType == vsomeip::message_type_e::MT_NOTIFICATION))
                && !endpointList.clientList.empty())
        {
            for(int i=0;i<endpointList.clientList.size();++i)
            {
                endpointList.clientList[i]->onMessageReceived(msg);
            }
        }
    });
}

void Application::onAvailabilityChanged(int16_t service, int16_t instance, bool availability)
{
    RegisteredEndpointList& endpointList = m_registeredEndpointList[service][instance];
    for(int i=0;i<endpointList.clientList.size();++i)
    {
        endpointList.clientList[i]->onAvailabilityChanged(availability);
    }
}

std::shared_ptr<Application> getApplication(const std::string &name)
{
    static std::unordered_map<std::string, std::shared_ptr<Application>> appMap;
    std::shared_ptr<Application>& app = appMap[name];
    if(!app)
        app = std::make_shared<Application>(name);
    return app;
}

}
