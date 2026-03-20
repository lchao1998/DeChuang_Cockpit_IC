#include "application.hpp"
#include "log.hpp"
#include "configuration/configuration.hpp"
#define  BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION 
#define  BOOST_THREAD_PROVIDES_EXECUTORS 
#define  BOOST_THREAD_USES_MOVE
#include <boost/thread/thread_pool.hpp>
#include <boost/stacktrace.hpp>
#include <thread>
#include <condition_variable>
#include <signal.h>

namespace AppFw
{

void crash_signal_handler(int signum) {
    ::signal(signum, SIG_DFL);

    if(signum == SIGSEGV || signum == SIGABRT)
    {
        LOG_FATAL <<" Application Signal["<<signum<<"]Backtrace: \n" <<boost::stacktrace::stacktrace();
    }
    ::raise(SIGABRT);
}

struct CrashHandler{
    CrashHandler(){
        ::signal(SIGSEGV, &crash_signal_handler);
        ::signal(SIGABRT, &crash_signal_handler);
        ::signal(SIGINT, &crash_signal_handler);
        ::signal(SIGTERM, &crash_signal_handler);
    }
}crashHandler;

class ConcurrentTaskQueue
{
public:
    explicit ConcurrentTaskQueue(const int& size):
        m_size(size),
        m_threadPool(size),
        m_count(0)
    {
    }

    ~ConcurrentTaskQueue()
    {

    }

    int size() const
    {
        return m_size;
    }

    void submit( std::function<void(void)> task)
    {
        std::unique_lock<std::mutex> locker(m_mtx);

        m_threadPool.submit([&, task](){
            task();
            std::lock_guard<std::mutex> locker(m_mtx);
            m_count--;
            m_cond.notify_one();
        });
        m_count++;
    }

    bool waitingForIdle(std::chrono::milliseconds waitTime)
    {
        std::unique_lock<std::mutex> locker(m_mtx);
        if(m_count >= m_size)
        {
            return (m_cond.wait_for(locker, waitTime) == std::cv_status::no_timeout);
        }
        else
            return true;
    }

private:
    int m_size;
    int m_count;
    std::mutex m_mtx;
    std::condition_variable m_cond;
    boost::basic_thread_pool m_threadPool;
};

struct ApplicationPrivate
{
    bool running;
    std::vector< std::shared_ptr<Module> > modules;
};

Application::Application(int argc, char** argv):
    m_p(new ApplicationPrivate)
{
    Configuration::get().parseFromCommandline(argc, argv);
    m_p->running = false;
}

Application::~Application()
{
    delete m_p;
}

void Application::registerModule(const char *name, std::shared_ptr<Module> module)
{
    if(m_p->running)
    {
        LOG_WARNING<<" You can not registerModule after application has called exec()!";
        return;
    }

    for(int i=0;i<m_p->modules.size();++i)
    {
        if(m_p->modules[i]->getName() == name)
        {
            LOG_WARNING<<"module:"<<name<<" has already registered, please check your code!";
            return ;
        }
    }

    module->setName(name);
    m_p->modules.push_back(module);
}

int Application::exec()
{
    const std::chrono::milliseconds blockMaxTime(Configuration::get().blockMaxTime);
    const std::chrono::milliseconds execInterval(Configuration::get().execInterval);
    const bool exitOnBlocked = (bool)Configuration::get().exitOnBlocked;
    int threadCount = Configuration::get().maxWorkingThreads;

    if(threadCount >= m_p->modules.size())
        threadCount = m_p->modules.size();

    assert(threadCount > 0);

    ConcurrentTaskQueue taskQueue(threadCount);
    m_p->running = true;

    while(m_p->running)
    {
        for(int i=0;i<m_p->modules.size();++i)
        {
            auto module = m_p->modules[i];

            bool taskQueueIdle = taskQueue.waitingForIdle(blockMaxTime);

            if(taskQueueIdle)
            {
                if(!module->isProcessing())
                    taskQueue.submit([module](){
                        module->process();
                    });
            }
            else
            {
                LOG_WARNING<<"Application waiting for task queue idle timeout";
                if(exitOnBlocked)
                {
                    LOG_WARNING<<"Application exiting because of it's blocked";
                    quit();
                }
            }
        }
        std::this_thread::sleep_for(execInterval);
    }
    LOG_INFO<<"Application quit..";
    return 0;
}

void Application::quit()
{
    m_p->running = false;
}

void Application::execOnce()
{

}
}
