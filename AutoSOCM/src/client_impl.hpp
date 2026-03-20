#ifndef AUTOSOCM_CLIENT_IMPL_HPP
#define AUTOSOCM_CLIENT_IMPL_HPP

#include "client.hpp"
#include "message.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace Protocol {
class Message;
class Endpoint;
}

namespace AutoSOCM
{

class ClientImpl:public Client
{
public:
    ClientImpl(uint16_t service, uint16_t instance, const std::string& appName);

    ~ClientImpl();

     bool isAvailible() const;

     const Data& getAttribute(const std::string& name);

     void invokeMethodAsync(const std::string& name, AsyncDataCallbackHandler returnHandler = nullptr);

     void invokeMethodAsync(const std::string& name, Data parameter, AsyncDataCallbackHandler returnHandler = nullptr);

     void setAvailabilityChangedHandler(AvailabilityChangedHandler handler);

     void setEventTriggerHandler(const std::string& name, AsyncDataCallbackHandler triggerHandler);

     void setPollable(bool pollable);

     bool poll();
private:
    void onAvailabilityChanged(bool availability);

    void onMessageReceived(std::shared_ptr<Protocol::Message> msg);

private:
    bool m_availability;
    std::string m_appName;
    std::mutex m_mtx;
    std::shared_ptr<Protocol::Endpoint> m_endpoint;
    std::mutex m_invokeMethodMtx;
    AvailabilityChangedHandler m_availabilityChangedHandler;
    std::unordered_map<std::string, AsyncDataCallbackHandler> m_eventTriggerHandlerMap;
    std::queue<std::pair<uint16_t, AsyncDataCallbackHandler>> m_methodReturnHandlerQueue;
    std::unordered_map<std::string, Data> m_attrMap;
};

}

#endif
