#ifndef PROTOCOL_ENDPOINT_HPP
#define PROTOCOL_ENDPOINT_HPP

#include "message.hpp"
#include <string>
#include <memory>
#include <mutex>
#include <queue>
#include <functional>

namespace vsomeip
{
class application;
class runtime;
class message;
}

namespace Protocol {

typedef std::function<void(std::shared_ptr<Message>)> MessageHandler;
typedef std::function<void(bool)> AvailabilityChangedHandler;

class Endpoint
{
    friend class Application;

public:
    enum class Type
    {
        Client,
        Service
    };

    Endpoint(const std::string& appName, int16_t service, int16_t instance, Type type);

    Type getType() const;

    uint16_t getService() const;

    uint16_t getInstance() const;

    std::shared_ptr<Message> createRequest(MessageID msgID, const void* msgPayload, size_t msgPayloadSize, bool reliable = false);

    std::shared_ptr<Message> createResponse(std::shared_ptr<Message> reqMsg, const void *msgData, size_t msgDataLength);

    std::shared_ptr<Message> createNotification(MessageID msgID, const void* msgPayload, size_t msgPayloadSize, ClientID client = 0);

    void sendMessage(const std::shared_ptr<Message>& msg);

    void setMessageHandler(MessageHandler handler);

    void setAvailabilityChangedHandler(AvailabilityChangedHandler handler);

    void setPollable(bool pollable);

    bool poll();

private:
    void onMessageReceived( std::shared_ptr<vsomeip::message> msg);
    void onAvailabilityChanged( bool availability );

private:
    uint16_t m_service;
    uint16_t m_instance;
    Type m_type;
    bool m_pollable;
    std::mutex m_mtx;
    std::string m_appName;
    std::shared_ptr<vsomeip::runtime> m_rtm;
    std::shared_ptr<vsomeip::application> m_app;
    std::queue<std::shared_ptr<Message>> m_msgQueue;
    MessageHandler m_msgHandler;
    AvailabilityChangedHandler m_availabilityChangedHandler;
};
}
#endif
