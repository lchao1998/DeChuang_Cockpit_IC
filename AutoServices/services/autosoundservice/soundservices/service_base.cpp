#include "service_base.hpp"

namespace AutoSoundService
{

    ServiceBase::ServiceBase(uint16_t service, uint16_t instance, std::string tag)
        : m_serviceName(std::move(tag)),
          m_service(AutoSOCM::getApplication()->createService(service, instance))
    {

    }

    ServiceBase::~ServiceBase() = default;


}