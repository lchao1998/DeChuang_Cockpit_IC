#ifndef AUTOSOCM_SERVICE_HPP
#define AUTOSOCM_SERVICE_HPP

#include "data.hpp"
#include <functional>
namespace AutoSOCM {

class Service
{
public:
    typedef std::function<void(const Data&)> MethodReturnHandler;
    typedef std::function<void(const Data&/*parameter*/, MethodReturnHandler/*returnHandler*/,uint16_t/*clientID*/)> InvokeMethodHandler;

    virtual ~Service(){}

    virtual void initAttribute(const std::string& name, Data defaultValue) = 0;

    virtual const Data& getAttribute(const std::string& name) = 0;

    virtual void setAttribute(const std::string& name, Data value, bool forceChanged = false) = 0;

    virtual void notifyEvent(const std::string& name, Data parameter = Data()) = 0;

    virtual void setInvokeMethodHandler(const std::string& name,  InvokeMethodHandler handler) = 0;

    virtual void setPollable(bool pollable) = 0;

    virtual bool poll() = 0;
};
}
#endif
