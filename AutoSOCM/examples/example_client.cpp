#include "application.hpp"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
    bool pollable = false;
    unsigned int interval = 10;
    std::shared_ptr<AutoSOCM::Application> app = AutoSOCM::getApplication();
    std::shared_ptr<AutoSOCM::Client> client = app->createClient(0x01,0x01);

    if(argc >= 2 && !std::strncmp(argv[1],"--pollable=",11))
    {
        char* arg;
        arg = argv[1]+11;
        pollable = (bool)atoi(arg);
        if(argc >= 3 && !std::strncmp(argv[1],"--interval=",11))
        {
            arg = argv[2]+11;
            interval = atoi(arg);
        }
    }

    client->setAvailabilityChangedHandler([&](bool availability){
        if(availability)
        {
            client->invokeMethodAsync("SetAttribute",  1234, [&](const AutoSOCM::Data& /*returnValue*/){
                std::cout<< "Method \"SetAttribute\" returned: Attribute=" << client->getAttribute("Attribute").toString() <<std::endl;
            });

            AutoSOCM::DataArray parameter;
            parameter.push_back(1);
            parameter.push_back(2);
            parameter.push_back(3);
            parameter.push_back(4);
            parameter.push_back(5);
            client->invokeMethodAsync("ListReverse",  parameter, [&](const AutoSOCM::Data& returnValue){
                AutoSOCM::DataArray arr = returnValue.toArray();
                std::cout << "Method \"ListReverse\" returned: ";
                for(int i=0;i<arr.size();++i)
                    std::cout << arr[i].toString()<<" ";
                std::cout << std::endl;
            });

            client->invokeMethodAsync("StartNotify");
        }
    });

    client->setEventTriggerHandler("Event", [&](const AutoSOCM::Data& parameter){
        std::cout<< "Event \"Event\" triggered: parameter="<<parameter.toString()<<std::endl;
    });

    client->setEventTriggerHandler("Stop", [&](const AutoSOCM::Data& parameter){
        app->stop();
    });

    std::cout << "Client pollable =" << std::boolalpha << pollable <<std::endl;

    client->setPollable(pollable);

    if(pollable)
    {
        app->startAsync();
        while(app->isRunning())
        {
            client->poll();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }
    else
        app->start();
}
