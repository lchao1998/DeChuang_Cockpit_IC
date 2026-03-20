#include "module.hpp"
#include "log.hpp"
#include <chrono>
#include <vector>

#define MAX_MODULE_PROCESS_MS 100/*ms*/

namespace AppFw
{

struct ModulePrivate
{
    std::string name;
    std::vector<Module::Task> tasks;
    std::chrono::steady_clock::time_point processBeginTp;
    bool isProcessing;
};

Module::Module():
    m_p(new ModulePrivate)
{
    m_p->isProcessing = false;
}

Module::~Module()
{
    delete m_p;
}

void Module::setName(const char* name)
{
    m_p->name = name;
}

std::string Module::getName() const
{
    return m_p->name;
}

bool Module::isProcessing() const
{
    return m_p->isProcessing;
}

bool Module::isBlocked() const
{
    if(m_p->isProcessing && (std::chrono::steady_clock::now() - m_p->processBeginTp) >= std::chrono::milliseconds(MAX_MODULE_PROCESS_MS))
        return true;
    return false;
}

void Module::postTask(Task task)
{
    m_p->tasks.push_back(task);
}

void Module::onProcess()
{

}

void Module::process()
{
    m_p->isProcessing = true;
    m_p->processBeginTp = std::chrono::steady_clock::now();

    onProcess();

    int count = m_p->tasks.size();
    for(int i=0;i<count;++i)
    {
        m_p->tasks[i]();
    }
    if(count > 0)
        m_p->tasks.erase(m_p->tasks.begin(),m_p->tasks.begin() + count);

    m_p->isProcessing = false;
}

}


