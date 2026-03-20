#include "client_impl.hpp"
#include "application.hpp"
#include "protocol/runtime.hpp"
#include "protocol/application.hpp"
#include "protocol/endpoint.hpp"
#include "protocol/message.hpp"
#include "assert.h"

namespace AutoSOCM {

ClientImpl::ClientImpl(uint16_t service, uint16_t instance, const std::string& appName):
    m_appName(appName),
    m_endpoint(Protocol::Runtime::get()->getApplication(appName)->createEndpoint(service, instance, Protocol::Endpoint::Type::Client)),
    m_availability(false)
{
    m_endpoint->setAvailabilityChangedHandler( std::bind(&ClientImpl::onAvailabilityChanged, this, std::placeholders::_1));
    m_endpoint->setMessageHandler( std::bind(&ClientImpl::onMessageReceived, this, std::placeholders::_1) );
}

ClientImpl::~ClientImpl()
{
    Protocol::Runtime::get()->getApplication(m_appName)->removeEndpoint(m_endpoint);
}

bool ClientImpl::isAvailible() const
{
    return m_availability;
}

const Data& ClientImpl::getAttribute(const std::string& name)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    return m_attrMap[name];
}

void ClientImpl::invokeMethodAsync(const std::string &name, Client::AsyncDataCallbackHandler returnHandler)
{
    invokeMethodAsync(name, Data(), returnHandler);
}

void ClientImpl::invokeMethodAsync(const std::string &name, Data parameter, Client::AsyncDataCallbackHandler returnHandler)
{
    DataArray arr;
    arr.push_back(name);
    arr.push_back(std::move(parameter));
    Data msgData( arr );

    std::shared_ptr<Protocol::Message> request = m_endpoint->createRequest(MESSAGE_ID_INVOKE_METHOD, msgData.getData(), msgData.getSize());
    m_endpoint->sendMessage( request );

    std::unique_lock<std::mutex> locker(m_mtx);
    if(returnHandler)
    {
        m_methodReturnHandlerQueue.push( std::make_pair(request->getSession(), returnHandler) );
    }
}

void ClientImpl::setAvailabilityChangedHandler(Client::AvailabilityChangedHandler handler)
{
    m_availabilityChangedHandler = handler;
}

void ClientImpl::setEventTriggerHandler(const std::string &name, Client::AsyncDataCallbackHandler triggerHandler)
{
    std::unique_lock<std::mutex> locker(m_mtx);
    m_eventTriggerHandlerMap[name] = triggerHandler;
}

void ClientImpl::setPollable(bool pollable)
{
    m_endpoint->setPollable(pollable);
}

bool ClientImpl::poll()
{
    return m_endpoint->poll();
}

void ClientImpl::onAvailabilityChanged(bool availability)
{
    if(availability != m_availability)
    {
        if(availability)
        {
            std::shared_ptr<Protocol::Message> request = m_endpoint->createRequest(MESSAGE_ID_GET_ATTRIBUTE_LIST, nullptr, 0);
            m_endpoint->sendMessage(request);
        }
        m_availability = availability;
        if(m_availabilityChangedHandler)
            m_availabilityChangedHandler(availability);
    }
}

void ClientImpl::onMessageReceived(std::shared_ptr<Protocol::Message> msg)
{
    Protocol::MessageType msgType = msg->getType();

    Data msgData( static_cast<const char*>(msg->getPayload()), msg->getPayloadSize());

    if(msgType == Protocol::MessageType::Response)
    {
        switch (msg->getID()) 
        {
            case MESSAGE_ID_GET_ATTRIBUTE_LIST:
                {
                    m_attrMap = msgData.toMap();
                }break;

            case MESSAGE_ID_INVOKE_METHOD:
                {
                    while(!m_methodReturnHandlerQueue.empty())
                    {
                        uint16_t session = m_methodReturnHandlerQueue.front().first;
                        AsyncDataCallbackHandler returnHandler = m_methodReturnHandlerQueue.front().second;
                        if(session == msg->getSession())
                        {
                            returnHandler(msgData);
                        }
                        else if(session > msg->getSession())
                            break;
                        m_methodReturnHandlerQueue.pop();
                    }
                }break;
            default: break;
        }
    }
    else if(msgType == Protocol::MessageType::Notification)
    {
        switch(msg->getID())
        {
            case MESSAGE_ID_ATTRIBUTE_CHANGED:
                {
                    std::lock_guard<std::mutex> locker(m_mtx);
                    DataArray arr = msgData.toArray();
                    if(arr.size() >= 2)
                        m_attrMap[arr[0].toString()] = std::move(arr[1]);
                }break;
            case MESSAGE_ID_EVENT_TRIGGER:
                {
                    std::lock_guard<std::mutex> locker(m_mtx);
                    DataArray arr = msgData.toArray();
                    if(arr.size() >= 2)
                    {
                        AsyncDataCallbackHandler handler = m_eventTriggerHandlerMap[arr[0].toString()];
                        if(handler)
                            handler(arr[1]);
                    }
                }break;
            default: break;
        }
    }
}
}
