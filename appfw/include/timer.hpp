/**
* @file         log.hpp
* @brief        log class
* @details      Create Timer in your Module, because Timer's scheduling relay on Module.
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
#ifndef APPFW_TIMER_HPP
#define APPFW_TIMER_HPP

#include <functional>

namespace AppFw {

class Module;

class Timer
{
public:
    typedef std::function<void(void)> Callback;

    Timer(Module* module, const unsigned int& interval, Callback callback);

    ~Timer();

    void start();

    void stop();

private:
    struct TimerPrivate* m_p;
};
} /* namespace AppFw */

#endif /* APPFW_TIMER_HPP */
