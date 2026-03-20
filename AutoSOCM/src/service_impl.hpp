#ifndef AUTOSOCM_SERVICE_IMPL_HPP
#define AUTOSOCM_SERVICE_IMPL_HPP

#include "service.hpp"
#include "message.hpp"
#include <mutex>
#include <memory>

namespace Protocol
{
class Endpoint;
class Message;
}

namespace AutoSOCM
{

class ServiceImpl: public Service
{
public:
    ServiceImpl(uint16_t service, uint16_t instance, const std::string& appName);

    ~ServiceImpl();

    void initAttribute(const std::string& name, Data defaultValue);

    const Data& getAttribute(const std::string& name);

    void setAttribute(const std::string& name, Data value, bool forceChanged = false);

    void notifyEvent(const std::string& name, Data parameter = Data());

    void setInvokeMethodHandler(const std::string& name,  InvokeMethodHandler handler);

    void setPollable(bool pollable);

    bool poll();

private:
    void onMessageReceived(std::shared_ptr<Protocol::Message> msg);

private:
    std::shared_ptr<Protocol::Endpoint> m_endpoint;
    std::string m_appName;
    std::mutex m_mtx;
    std::unordered_map<std::string, InvokeMethodHandler> m_invokeMethodHandlerMap;
    DataMap m_attrMap;
};

}
#endif
