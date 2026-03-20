/**
* @file         configeration.hpp
* @brief        configer class
* @details      parse commandline to initialize application parameters
* @author   
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
#ifndef APPFW_CONFIGURATION_HPP
#define APPFW_CONFIGURATION_HPP

namespace AppFw{

#define APP_EXEC_INTERVAL_DEFAULT 1
#define APP_WORK_THREADS_MAX 16
#define LOG_LEVEL_DEFAULT 0
#define APP_EXIT_ON_BLOCKED 0
#define APP_BLOCK_MAX_TIME 2000

class Configuration
{
public:
    int execInterval{APP_EXEC_INTERVAL_DEFAULT};

    int maxWorkingThreads{APP_WORK_THREADS_MAX};

    int logSeverity{LOG_LEVEL_DEFAULT};

    int exitOnBlocked{APP_EXIT_ON_BLOCKED};

    int blockMaxTime{APP_BLOCK_MAX_TIME};

    void parseFromCommandline(int argc, char** argv);

    static Configuration& get();
};
}
#endif
