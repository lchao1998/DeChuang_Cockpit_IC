/**
* @file         autologiccontroller.cpp
* @brief        main application entry
* @details      Application is our app's entry,  at least one Module should be register to Application
* @author       xu.qiang@zlingsmart.com
* @date         2022-3-1
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/03/01  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#include "appfw/application.hpp"
#include "module_define.h"
#include "gaugemodule/gauge_logic_controller.hpp"
#include "domainmodule/domain_logic_controller.hpp"
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
    logInfo("[ClusterController] Catch Signal %d Stop Task", sig);

    g_App->quit();

    AutoSOCM::getApplication()->stop();
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

    g_App = new AppFw::Application(argc, argv);
    if (g_App != NULL)
    {
        g_App->registerModule<GaugeLogicController>(MODULE_NAME_GAUGE);
        g_App->registerModule<DomainController>(MODULE_NAME_DOMAIN);

        AutoSOCM::getApplication()->startAsync();

        g_App->exec();
    }
    else
    {
        logError("[AutoLogicController] Create Application Error");
    }
    return 0;
}
