#include "application.hpp"
#include <thread>
#include <algorithm> //std::reverse

std::shared_ptr<AutoSOCM::Service> service;

void SetAttribute(const AutoSOCM::Data& parameter, AutoSOCM::Service::MethodReturnHandler returnHandler, uint16_t /*client*/)
{
    bool ok = false;
    int attrValue = parameter.toInt(&ok);
    if(ok)
    {
        service->setAttribute("Attribute", attrValue);
        returnHandler(true);
    }
}

void ListReverse(const AutoSOCM::Data& parameter, AutoSOCM::Service::MethodReturnHandler returnHandler, uint16_t /*client*/)
{
    AutoSOCM::DataArray arr = parameter.toArray();

    std::reverse(std::begin(arr), std::end(arr));

    returnHandler(arr);
}

void StartNotify(const AutoSOCM::Data& /*parameter*/, AutoSOCM::Service::MethodReturnHandler /*returnHandler*/, uint16_t client)
{
    for(int i=0;i<5;++i)
        service->notifyEvent("Event", i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    service->notifyEvent("Stop", client);
}

int main()
{
    service = AutoSOCM::getApplication()->createService(0x01,0x01);

    service->initAttribute("Attribute", 0);

    service->setInvokeMethodHandler("SetAttribute", SetAttribute);
    service->setInvokeMethodHandler("ListReverse", ListReverse);
    service->setInvokeMethodHandler("StartNotify", StartNotify);
    AutoSOCM::getApplication()->start();
}