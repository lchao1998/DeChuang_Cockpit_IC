#ifndef AUTOSOCM_CLIENT_INTERFACE_HPP
#define AUTOSOCM_CLIENT_INTERFACE_HPP

#include "data.hpp"
#include <functional>
#include <chrono>

namespace AutoSOCM {

class Client
{
public:
    typedef std::function<void(bool)> AvailabilityChangedHandler;
    typedef std::function<void(const Data&)> AsyncDataCallbackHandler;

    virtual ~Client(){}

    virtual bool isAvailible() const = 0;

    virtual const Data& getAttribute(const std::string& name) = 0;

    virtual void invokeMethodAsync(const std::string& name, AsyncDataCallbackHandler returnHandler = nullptr) = 0;

    virtual void invokeMethodAsync(const std::string& name, Data parameter, AsyncDataCallbackHandler returnHandler = nullptr) = 0;

    virtual void setAvailabilityChangedHandler(AvailabilityChangedHandler handler) = 0;

    virtual void setEventTriggerHandler(const std::string& name, AsyncDataCallbackHandler triggerHandler) = 0;

    virtual void setPollable(bool pollable) = 0;

    virtual bool poll() = 0;
};
}
#endif
