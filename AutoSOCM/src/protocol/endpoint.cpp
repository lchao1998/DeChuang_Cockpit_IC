#include "endpoint.hpp"
#include "common.hpp"
#include <vsomeip/vsomeip.hpp>
#include <cstring>
#include <iostream>
#include <thread>

namespace Protocol {

class MessageImpl: public Message
{
public:
     explicit MessageImpl( std::shared_ptr<vsomeip::message> msg): ipmsg(msg)
    {}

    MessageImpl( std::shared_ptr<vsomeip::message> msg, MessageID msgID, const void* payload, size_t payloadSize, ClientID client = 0): ipmsg(msg)
    {
        size_t msgSize = sizeof(MessageID) + payloadSize;
        vsomeip::byte_t msgData[msgSize];
        std::memcpy(msgData, &msgID, sizeof(MessageID));
        std::memcpy(msgData + sizeof(MessageID), payload, payloadSize);
        ipmsg->get_payload()->set_data(msgData, msgSize);
        if(client)
            ipmsg->set_client(client);
    }

    ~MessageImpl(){}

    bool isVaild()
    {
        MessageID id = getID();
        MessageType type = getType();
        return (type != MessageType::Unknown) && (id != InvalidMessageID);
    }

    MessageType getType() override
    {
        switch(ipmsg->get_message_type())
        {
        case vsomeip::message_type_e::MT_REQUEST: return MessageType::Request;
        case vsomeip::message_type_e::MT_RESPONSE: return MessageType::Response;
        case vsomeip::message_type_e::MT_NOTIFICATION: return MessageType::Notification;
        default:return MessageType::Unknown;
        }
    }

    MessageID getID()
    {
        MessageID id = InvalidMessageID;
        std::shared_ptr<vsomeip::payload> pl = ipmsg->get_payload();
        if(pl && pl->get_length() >= sizeof(MessageID))
        {
            std::memcpy(&id, pl->get_data(), sizeof(MessageID));
        }
        return id;
    }

    ClientID getClient() override
    {
        return ipmsg->get_client();
    }

    MessageSession getSession()
    {
        return ipmsg->get_session();
    }

    const void* getPayload() override
    {
        return ipmsg->get_payload()->get_data() + sizeof(MessageID);
    }

    size_t getPayloadSize() override
    {
        return ipmsg->get_payload()->get_length() - sizeof(MessageID);
    }

    std::shared_ptr<vsomeip::message> ipmsg;
};

Endpoint::Endpoint(const std::string &appName, int16_t service, int16_t instance, Endpoint::Type type) :
    m_appName(appName),
    m_rtm(vsomeip::runtime::get()),
    m_app(m_rtm->get_application(appName)),
    m_service(service),
    m_instance(instance),
    m_type(type),
    m_pollable(false)
{
}

Endpoint::Type Endpoint::getType() const
{
    return m_type;
}

uint16_t Endpoint::getService() const
{
    return m_service;
}

uint16_t Endpoint::getInstance() const
{
    return m_instance;
}

std::shared_ptr<Message> Endpoint::createRequest(MessageID msgID, const void *msgPayload, size_t msgPayloadSize, bool reliable)
{
    std::shared_ptr<vsomeip::message> msg = m_rtm->create_request(reliable);
    return std::make_shared<MessageImpl>(msg, msgID, msgPayload, msgPayloadSize);
}

std::shared_ptr<Message> Endpoint::createResponse(std::shared_ptr<Message> reqMsg, const void *msgData, size_t msgDataLength)
{
    std::shared_ptr<MessageImpl> reqMsgImpl = std::static_pointer_cast<MessageImpl>(reqMsg);
    std::shared_ptr<vsomeip::message> msg = m_rtm->create_response(reqMsgImpl->ipmsg);
    return std::make_shared<MessageImpl>(msg, reqMsgImpl->getID(), msgData, msgDataLength);
}

std::shared_ptr<Message> Endpoint::createNotification(MessageID msgID, const void* msgPayload, size_t msgPayloadSize, ClientID client)
{
    std::shared_ptr<vsomeip::message> msg = m_rtm->create_notification();

    return std::make_shared<MessageImpl>(msg, msgID, msgPayload, msgPayloadSize, client);
}

void Endpoint::sendMessage(const std::shared_ptr<Message> &msg)
{
    std::shared_ptr<MessageImpl> msgImpl = std::static_pointer_cast<MessageImpl>(msg);

    switch (msgImpl->getType()) {
    case MessageType::Request:
    case MessageType::Response:
    {
        msgImpl->ipmsg->set_service(m_service);
        msgImpl->ipmsg->set_instance(m_instance);
        msgImpl->ipmsg->set_method(defaultMethodID);
        msgImpl->ipmsg->set_reliable(false);
        m_app->send(msgImpl->ipmsg);
    }break;
    case MessageType::Notification:
    {
        if(msg->getClient())
        {
            m_app->notify_one(m_service, m_instance, defaultEventID, msgImpl->ipmsg->get_payload(), msgImpl->ipmsg->get_client());
        }
        else
        {
            m_app->notify(m_service, m_instance, defaultEventID, msgImpl->ipmsg->get_payload());
        }

    }break;
    default: return;
    }
}

void Endpoint::setMessageHandler(MessageHandler handler)
{
    m_msgHandler = handler;
}

void Endpoint::setAvailabilityChangedHandler(AvailabilityChangedHandler handler)
{
    m_availabilityChangedHandler = handler;
}

void Endpoint::setPollable(bool pollable)
{
    if(m_pollable != pollable)
    {
        if(!pollable)
            poll();
        m_pollable = pollable;
    }
}

bool Endpoint::poll()
{
    std::lock_guard<std::mutex> locker(m_mtx);
    bool hasMsg = false;
    while(m_pollable && m_msgHandler && !m_msgQueue.empty())
    {
        m_msgHandler(m_msgQueue.front());
        m_msgQueue.pop();
        hasMsg = true;
    }
    return hasMsg;
}

void Endpoint::onMessageReceived(std::shared_ptr<vsomeip::message> msg)
{
    std::shared_ptr<MessageImpl> msgImpl = std::make_shared<MessageImpl>(msg);
    if(msgImpl->isVaild())
    {
        std::lock_guard<std::mutex> locker(m_mtx);
        if(m_pollable)
        {
            m_msgQueue.push(msgImpl);
        }
        else if(m_msgHandler)
        {
            m_msgHandler(msgImpl);
        }
    }
}

void Endpoint::onAvailabilityChanged(bool availability)
{
    if(m_availabilityChangedHandler)
        m_availabilityChangedHandler(availability);
}

}
