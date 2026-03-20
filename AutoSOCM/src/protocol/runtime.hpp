#ifndef PROTOCOL_RUNTIME_HPP
#define PROTOCOL_RUNTIME_HPP

#include <string>
#include <memory>
#include <unordered_map>

namespace Protocol {

class Application;

class Runtime
{
public:
    static std::shared_ptr<Runtime> get();

    std::shared_ptr<Application> createApplication(const std::string& name);

    std::shared_ptr<Application> getApplication(const std::string& name);

private:
    std::unordered_map<std::string, std::shared_ptr<Application>> m_appMap;
};
}
#endif
