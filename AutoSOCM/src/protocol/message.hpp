#ifndef PROTOCOL_MESSAGE_HPP
#define PROTOCOL_MESSAGE_HPP

#include <sys/types.h>
#include <stdint.h>

namespace Protocol {

enum class MessageType
{
    Unknown,
    Request,
    Response,
    Notification
};

typedef uint16_t ClientID;

typedef uint16_t MessageSession;

typedef uint32_t MessageID;

const MessageID InvalidMessageID = 0;

class Message
{
public:
    virtual ~Message(){}

    virtual MessageType getType() = 0;

    virtual MessageID getID() = 0;

    virtual ClientID getClient() = 0;

    virtual MessageSession getSession() = 0;

    virtual const void* getPayload() = 0;

    virtual size_t getPayloadSize() = 0;
};

}

#endif
