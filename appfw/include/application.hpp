/**
* @file         Application.hpp
* @brief        main application entry
* @details      Application is our app's entry,  at least one Module should be register to Application
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

#ifndef APPFW_APPLICATION_HPP
#define APPFW_APPLICATION_HPP

#include <memory>
#include "module.hpp"

namespace AppFw
{
class Application
{
public:
    /**
     * @fn Application
     * @brief Application Constructor, the commandline params should be passed.
     * @param argc Commandline argc
     * @param argv Commandline argv
     */
    Application(int argc, char** argv);

    ~Application();


    template <typename ModuleType>
    void registerModule(const char* moduleName)
    {
        static_assert(std::is_base_of<Module, ModuleType>::value, "ModuleType invalid");
        std::shared_ptr<Module> module = std::make_shared<ModuleType>();
        registerModule(moduleName, module);
    }

    /**
     * @fn registerModule
     * @brief registerModule Register a module to application, call this method after Application::exec() is not allowed,
     * if a registered module with a same name, nothing will happen.
     * @param name module name
     * @param module module pointer
     */
    void registerModule(const char* name, std::shared_ptr<Module> module);

    /**
     * @fn exec
     * @brief exec Application start a loop to call each registered module's Module::process(),
     * @return return 0;
     */
    int exec();

    void quit();

protected:
    void execOnce();

private:
    struct ApplicationPrivate* m_p;
};
}

#endif // !FW_APPLICATION_HPP
