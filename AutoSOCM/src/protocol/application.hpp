#ifndef PROTOCOL_APPLICATION_HPP
#define PROTOCOL_APPLICATION_HPP

#include "endpoint.hpp"
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <boost/asio/io_service.hpp>

namespace Protocol {

class Application
{
public:
    Application(const std::string& name = "");

    ~Application();

    std::string getName() const;

    std::shared_ptr<Endpoint> createEndpoint(int16_t service, int16_t instance, Endpoint::Type type);

    void removeEndpoint(std::shared_ptr<Endpoint> endpoint);

    void start();

    void stop();

private:
    void onMessageReceived(const std::shared_ptr<vsomeip::message>& msg);
    void onAvailabilityChanged(int16_t service, int16_t instance, bool availability);

private:
    std::string m_name;
    std::shared_ptr<vsomeip::runtime> m_rtm;
    std::shared_ptr<vsomeip::application> m_app;
    std::mutex m_mtx;
    std::mutex m_runMtx;
    std::thread m_runThread;
    std::condition_variable m_runCondition;
    boost::asio::io_service m_io;
    typedef std::vector<std::shared_ptr<Endpoint>> EndpointList;
    struct RegisteredEndpointList
    {
        std::shared_ptr<Endpoint> service;
        EndpointList clientList;
    };

    std::unordered_map<uint16_t, std::unordered_map<uint16_t, RegisteredEndpointList>> m_registeredEndpointList;
};

}
#endif
