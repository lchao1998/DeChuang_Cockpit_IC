#include "runtime.hpp"
#include "application.hpp"

namespace Protocol {

std::shared_ptr<Runtime> Runtime::get()
{
    static std::shared_ptr<Runtime> rtm = std::shared_ptr<Runtime>( new Runtime );
    return rtm;
}

std::shared_ptr<Application> Runtime::createApplication(const std::string &name)
{
    std::shared_ptr<Application>& app = m_appMap[name];
    if(!app)
        app = std::make_shared<Application>(name);

    return app;
}

std::shared_ptr<Application> Runtime::getApplication(const std::string &name)
{
    return m_appMap[name];
}

}
