#include "timer.hpp"
#include "module.hpp"
#include <chrono>

namespace AppFw {

struct TimerPrivateDestroyed{};
struct TimerPrivate
{
    Module* module;
    bool running;
    std::chrono::steady_clock::time_point begin;
    Timer::Callback callback;
    std::chrono::milliseconds interval;
    std::shared_ptr<TimerPrivateDestroyed> destroy;
    void onTiming()
    {
        if(running)
        {
            auto now = std::chrono::steady_clock::now();
            if((now-begin)>=interval && callback)
            {
                callback();
                begin = now;
            }

            std::weak_ptr<TimerPrivateDestroyed> wp = destroy;
            module->postTask([&,wp](){
                if(!wp.expired())
                    onTiming();
            });
        }
    }
};

Timer::Timer(Module *module, const unsigned int &interval, Callback callback):
    m_p(new TimerPrivate)
{
    m_p->module = module;
    m_p->running = false;
    m_p->interval = std::chrono::milliseconds(interval);
    m_p->callback = callback;
    m_p->destroy = std::make_shared<TimerPrivateDestroyed>();
}

Timer::~Timer() {
    stop();
    delete m_p;
}

void Timer::start()
{
    m_p->begin = std::chrono::steady_clock::now();
    if(!m_p->running)
    {
        m_p->running  =true;
        m_p->onTiming();
    }
}

void Timer::stop()
{
    m_p->running = false;
}

} /* namespace Fw */



