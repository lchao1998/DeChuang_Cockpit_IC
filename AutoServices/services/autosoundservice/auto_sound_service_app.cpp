#include "Log/log.hpp"
#include "appfw/include/application.hpp"
#include "AutoSOCM/include/application.hpp"
#include "soundmanager/sound_manager.hpp"

#include <csignal>

AppFw::Application *g_App = nullptr;

/**
* @fn signal_handler
* @brief catch signal callback
* @param int sig: signal id
* @return:void
*/
void signal_handler(int sig)
{
    logInfo("auto sound service app Catch Signal %d Stop Task", sig);
    if(SIGABRT == sig)
    {
        std::abort();
    }

    if(AutoSOCM::getApplication()->isRunning())
    {
        AutoSOCM::getApplication()->stop();
    }
    g_App->quit();
}

// Function : main
// Note     : main
//
// @param int argc 
// @param char* argv[]    
//
// @return int
int main(int argc, char* argv[])
{
    signal(SIGINT,  &signal_handler);
    signal(SIGABRT, &signal_handler);
    signal(SIGTERM, &signal_handler);

    g_App = new AppFw::Application(argc, argv);

    if (g_App != nullptr)
    {

        logInfo("SoundServiceApp init main !!!\n");
        g_App->registerModule<AutoSoundService::SoundManager>(AutoSoundService::SOC_SOUND_SERVICE_MANAGER_NAME.c_str());

        AutoSOCM::getApplication()->startAsync();
        g_App->exec();

        logInfo("SoundServiceApp init exec !!!\n");
    }
    else
    {
        logInfo("SoundServiceApp Create Application Error\n");
    }
    return 0;
}
