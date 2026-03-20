#include "service_impl.hpp"
#include "protocol/runtime.hpp"
#include "protocol/application.hpp"
#include "protocol/endpoint.hpp"
#include "protocol/message.hpp"
#include <iostream>

namespace AutoSOCM {

ServiceImpl::ServiceImpl(uint16_t service, uint16_t instance, const std::string &appName):
    m_appName(appName),
    m_endpoint(Protocol::Runtime::get()->getApplication(appName)->createEndpoint(service,instance, Protocol::Endpoint::Type::Service))
{
    m_endpoint->setMessageHandler( std::bind(&ServiceImpl::onMessageReceived, this, std::placeholders::_1) );
}

ServiceImpl::~ServiceImpl()
{
    Protocol::Runtime::get()->getApplication(m_appName)->removeEndpoint(m_endpoint);
}

void ServiceImpl::initAttribute(const std::string &name, Data defaultValue)
{
    std::lock_guard<std::mutex> m_locker(m_mtx);
    m_attrMap[name] = std::move(defaultValue);
}

const Data& ServiceImpl::getAttribute(const std::string &name)
{
    std::lock_guard<std::mutex> m_locker(m_mtx);
    return m_attrMap[name];
}

void ServiceImpl::setAttribute(const std::string &name, Data value, bool forceChanged)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    Data& attrValue = m_attrMap[name];
    if(forceChanged || (attrValue != value))
    {
        DataArray arr;
        arr.push_back(name);
        arr.push_back(std::move(value));
        Data msgData(arr);

        std::shared_ptr<Protocol::Message> notification =
                m_endpoint->createNotification(MESSAGE_ID_ATTRIBUTE_CHANGED, msgData.getData(), msgData.getSize());
        m_endpoint->sendMessage(notification);
        attrValue = std::move(arr[1]);
    }
}

void ServiceImpl::notifyEvent(const std::string &name, Data parameter)
{
    DataArray arr;
    arr.push_back(name);
    arr.push_back(std::move(parameter));
    Data msgData(arr);

    std::shared_ptr<Protocol::Message> notification =
            m_endpoint->createNotification(MESSAGE_ID_EVENT_TRIGGER, msgData.getData(), msgData.getSize());
    m_endpoint->sendMessage(notification);
}

void ServiceImpl::setInvokeMethodHandler(const std::string &name, Service::InvokeMethodHandler handler)
{
    m_invokeMethodHandlerMap[name] = handler;
}

void ServiceImpl::setPollable(bool pollable)
{
    m_endpoint->setPollable(pollable);
}

bool ServiceImpl::poll()
{
    return m_endpoint->poll();
}

void ServiceImpl::onMessageReceived(std::shared_ptr<Protocol::Message> msg)
{
    Protocol::MessageType msgType = msg->getType();

    Data msgData(static_cast<const char*>(msg->getPayload()), msg->getPayloadSize());

    if(msgType == Protocol::MessageType::Request)
    {
        switch (msg->getID())
        {
            case MESSAGE_ID_GET_ATTRIBUTE_LIST:
                {
                    std::unique_lock<std::mutex> locker(m_mtx);
                    Data attrList(m_attrMap);
                    std::shared_ptr<Protocol::Message> response = m_endpoint->createResponse(msg, attrList.getData(),attrList.getSize());
                    m_endpoint->sendMessage(response);
                }break;
            case MESSAGE_ID_INVOKE_METHOD:
                {
                    DataArray arr = msgData.toArray();
                    if(arr.size() >= 2)
                    {
                        MethodReturnHandler returnHandler = [&,msg](const Data& data )
                        {
                            std::shared_ptr<Protocol::Message> response = m_endpoint->createResponse(msg, data.getData(),data.getSize());
                            m_endpoint->sendMessage(response);
                        };

                        std::string methodName = arr[0].toString();

                        InvokeMethodHandler invokeMethodhandler = m_invokeMethodHandlerMap[methodName];
                        if(invokeMethodhandler)
                            invokeMethodhandler(arr[1], returnHandler, msg->getClient());
                    }
                }break;
            default:
                break;
            }
    }
}


}
