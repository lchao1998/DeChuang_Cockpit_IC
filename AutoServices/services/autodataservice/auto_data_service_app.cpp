/**
* @file         autodataserviceapp.cpp
* @brief        main application entry
* @details      Application is our app's entry,  at least one Module should be register to Application
* @author       xu.qiang@zlingsmart.com
* @date         2022-2-10
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/02/10  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#include "appfw/application.hpp"
#include "service_manager.hpp"
#include "log/log.hpp"
#include <thread>
#include <signal.h>

AppFw::Application * g_App = NULL;

/**
* @fn signal_handler
* @brief catch signal callback
* @param int sig: signal id
* @return:void
*/
void signal_handler(int sig)
{
    logInfo("autodataserviceapp Catch Signal %d Stop Task", sig);

    AutoSOCM::getApplication()->stop();

    g_App->quit();
}

/**
* @fn main
* @brief main
* @param int argc 
* @param char* argv[]  
* @return:int
*/
int main(int argc, char* argv[])
{
    signal(SIGINT,  &signal_handler);
    signal(SIGABRT, &signal_handler);
    signal(SIGKILL, &signal_handler);
    signal(SIGTERM, &signal_handler);

    g_App = new AppFw::Application(argc, argv);
    if (g_App != NULL)
    {
        logInfo("autodataserviceapp init main !!!\n");
        g_App->registerModule<AutoDataService::ServiceManager>(AutoDataService::SOC_AUTO_DATA_SERVICE_MANAGER_NAME.c_str());

        AutoSOCM::getApplication()->startAsync();
        g_App->exec();
        logInfo("autodataserviceapp init exec !!!\n");
    }
    else
    {
        logError("autodataserviceapp Create Application Error");
    }
    return 0;
}
