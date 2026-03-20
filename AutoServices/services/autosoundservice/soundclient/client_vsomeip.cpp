//
// Created by Administrator on 2022/8/9.
//

#include "client_vsomeip.hpp"
#include "Log/log.hpp"

#include <sstream>
#include <iomanip>
#include <future>

namespace AutoSoundService
{
    struct VSomeipClientPrivate
    {
        const uint16_t                          serviceId;
        const uint16_t                          instanceId;
        const uint16_t                          methodId;
        bool                                    use_tcp{false};
        std::atomic<bool>                       isInit{false};
        std::atomic<bool>                       isAvailable{false};

        std::shared_ptr<vsomeip::application>   app;
        std::shared_ptr<vsomeip::message>       request;

        explicit VSomeipClientPrivate(uint16_t _serviceId, uint16_t _instanceId, uint16_t _methodId)
            :   serviceId(_serviceId),
                instanceId(_instanceId),
                methodId(_methodId)
        { };
    };

    VSomeipClient::VSomeipClient(uint16_t serviceId, uint16_t instanceId, uint16_t methodId)
        : m_p(new VSomeipClientPrivate(serviceId, instanceId, methodId))
    {
        m_p->app        = ::vsomeip::runtime::get()->create_application();
        m_p->request    = ::vsomeip::runtime::get()->create_request(m_p->use_tcp);
    }

    VSomeipClient::~VSomeipClient()
    {
        if (m_p->isInit)
        {
            this->stop();
        }
        delete m_p;
    }

    bool VSomeipClient::init() {
        if (!m_p->app->init())
        {
            logError("Couldn't initialize application");
            m_p->isInit = false;
            return m_p->isInit;
        }

        logInfo("Client settings [protocol=%s]", (m_p->use_tcp ? "TCP" : "UDP"));

        m_p->app->register_state_handler(
                std::bind(
                        &VSomeipClient::onState,
                        this,
                        std::placeholders::_1));

        m_p->app->register_message_handler(
                vsomeip::ANY_SERVICE, m_p->instanceId, vsomeip::ANY_METHOD,
                std::bind(&VSomeipClient::onMessage,
                          this, std::placeholders::_1));

        m_p->request->set_service(m_p->serviceId);
        m_p->request->set_instance(m_p->instanceId);
        m_p->request->set_method(m_p->methodId);

        m_p->app->register_availability_handler(m_p->serviceId, m_p->instanceId,
                                            std::bind(&VSomeipClient::onAvailability,
                                                      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

        m_p->app->register_availability_handler(m_p->serviceId + 1, m_p->instanceId,
                                            std::bind(&VSomeipClient::onAvailability,
                                                      this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        m_p->isInit = true;
        return m_p->isInit;
    }

    void VSomeipClient::start()
    {
        std::thread([this](){
            this->m_p->app->start();
        }).detach();
    }

    void VSomeipClient::stop()
    {
        if(m_p->isInit)
        {
            m_p->isInit      = false;
            m_p->isAvailable = false;
            m_p->app->clear_all_handler();
            m_p->app->release_service(m_p->serviceId, m_p->instanceId);
            m_p->app->stop();
        }
    }

    bool VSomeipClient::send(const std::vector<uint8_t> &data)
    {
        if (data.empty() || !m_p->isAvailable)
        {
            return false;
        }

        std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
        its_payload->set_data(data);
        m_p->request->set_payload(its_payload);
        m_p->app->send(m_p->request);
        std::stringstream debugInfo;
        debugInfo << "Client/Session ["
                  << std::setw(4) << std::setfill('0') << std::hex << m_p->request->get_client()
                  << "/"
                  << std::setw(4) << std::setfill('0') << std::hex << m_p->request->get_session()
                  << "] sent a request to Service ["
                  << std::setw(4) << std::setfill('0') << std::hex << m_p->request->get_service()
                  << "."
                  << std::setw(4) << std::setfill('0') << std::hex << m_p->request->get_instance()
                  << "]"
                  << std::endl;
        logDebug("%s", debugInfo.str().c_str());
        return true;
    }

    void VSomeipClient::onState(vsomeip::state_type_e state)
    {
        if(vsomeip::state_type_e::ST_REGISTERED == state)
        {
            m_p->app->request_service(m_p->serviceId, m_p->instanceId);
        }
    }

    void VSomeipClient::onMessage(const std::shared_ptr<vsomeip::message> &response)
    {
        std::stringstream responseInfo;
        responseInfo << "Received a response from Service ["
                     << std::setw(4) << std::setfill('0') << std::hex << response->get_service()
                     << "."
                     << std::setw(4) << std::setfill('0') << std::hex << response->get_instance()
                     << "] to Client/Session ["
                     << std::setw(4) << std::setfill('0') << std::hex << response->get_client()
                     << "/"
                     << std::setw(4) << std::setfill('0') << std::hex << response->get_session()
                     << "]"
                     << std::endl;
        logInfo("%s", responseInfo.str().c_str());
        this->send(std::vector<uint8_t>{1, 2, 3});
    }

    void VSomeipClient::onAvailability(vsomeip::service_t service, vsomeip::instance_t instance, bool isAvailable)
    {
        std::stringstream responseInfo;
        responseInfo << "Service ["
                     << std::setw(4) << std::setfill('0') << std::hex << service << "." << instance
                     << "] is "
                     << (m_p->isAvailable ? "available." : "NOT available.")
                     << std::endl;
        logInfo("%s", responseInfo.str().c_str());

        if(service == m_p->serviceId && instance == m_p->instanceId)
        {
            if(m_p->isAvailable && !isAvailable)
            {
                m_p->isAvailable = false;
            }
            else if (isAvailable && !m_p->isAvailable)
            {
                m_p->isAvailable = true;
            }
        }
    }
}


