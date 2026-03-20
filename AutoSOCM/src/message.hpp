#ifndef AUTOSOCM_MESSAGE_HPP
#define AUTOSOCM_MESSAGE_HPP

#include <vector>
#include <unordered_map>
#include "protocol/message.hpp"

namespace AutoSOCM {

enum MessageID : Protocol::MessageID
{
    MESSAGE_ID_GET_ATTRIBUTE_LIST = 1,
    MESSAGE_ID_INVOKE_METHOD,
    MESSAGE_ID_ATTRIBUTE_CHANGED,
    MESSAGE_ID_EVENT_TRIGGER,
};
}
#endif
