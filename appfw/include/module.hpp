/**
* @file         log.hpp
* @brief        log class
* @details      Module is our logic entry, at least one module should be register to Application 
*               start to archive a module from derivering Module, override onProcess to start handle your logic processing.
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
#ifndef APPFW_MODULE_HPP
#define APPFW_MODULE_HPP

#include <string>
#include <memory>
#include <functional>

namespace AppFw
{
struct ModuleInfo;

class Module
{
    friend class Application;
public:
    typedef std::function<void(void)> Task;

    Module();

    virtual ~Module();

    /**
     * @brief setName
     * @param name
     */
    void setName(const char* name);

    /**
     * @brief getName get this module's name.
     * @return return module's name.
     */
    std::string getName() const;

    bool isProcessing() const;

    bool isBlocked() const;

    void postTask(Task task);

protected:
    virtual void onProcess();

    void setInfo(std::shared_ptr<ModuleInfo> info);

    std::shared_ptr<ModuleInfo> getInfo();
private:

    void process();

private:
    struct ModulePrivate* m_p;
};
}


#endif // ! APPFW_MODULE_HPP
