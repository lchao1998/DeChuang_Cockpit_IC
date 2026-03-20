/**
* @file         log.hpp
* @brief        Log API,
* @details      std::ostream style log: logDebug()<<"log msg";
*               printf style log: logDebug("%s", "log msg");
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
#ifndef LOG_HPP
#define LOG_HPP

#include "base.hpp"

extern Logger logDebug();
extern Logger logInfo();
extern Logger logWarning();
extern Logger logError();
extern Logger logFatal();

#define __PRINTF_STYLE_CHECK __attribute__ ((format (printf, 1, 2)))

extern void logDebug(const char* format, ...);   //__PRINTF_STYLE_CHECK;
extern void logInfo(const char* format, ...);    //__PRINTF_STYLE_CHECK;
extern void logWarning(const char* format, ...); //__PRINTF_STYLE_CHECK;
extern void logError(const char* format, ...);   //__PRINTF_STYLE_CHECK;
extern void logFatal(const char* format, ...);   //__PRINTF_STYLE_CHECK;



#endif // LOG_HPP
