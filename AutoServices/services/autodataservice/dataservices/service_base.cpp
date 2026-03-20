#include "service_base.hpp"
#include "string.h"

namespace AutoDataService
{
    ServiceBase::ServiceBase(ServiceInterface* manager, uint16_t service, uint16_t instance, const char* tag) :
            m_service_manager(manager),
            m_Service(AutoSOCM::getApplication()->createService(service, instance)),
            OUTPUT_TAG(tag)
    {
        // TODO Auto-generated constructor stub
    }
    
    ServiceBase::~ServiceBase()
    {
        OUTPUT_TAG  = NULL;
        m_service_manager = NULL;
    }

    std::shared_ptr<AutoSOCM::Service> ServiceBase::getService()
    {
        return m_Service;
    }

} 