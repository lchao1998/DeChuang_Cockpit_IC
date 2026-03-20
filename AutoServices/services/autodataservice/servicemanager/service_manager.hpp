/**
* @file         service_manager.hpp
* @brief        service manager
* @details      manager com data read and write ; manager sub service; message read and write;
                data dispatch and so on.
* @author       xu.qiang@zlingsmart.com
* @date         2022-6-17
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention 
* 
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/06/17  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma  once
#include <fstream>

#include <unistd.h>
#include <map>
#include <string>
#include <cstring>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "appfw/module.hpp"
#include "appfw/timer.hpp"
#include "service_base.hpp"
#include "domainservice/domain_service.hpp"
#include "gaugeservice/gauge_service.hpp"
#include <thread>
#include <condition_variable>
#include <mutex>
#include <linux/can.h>
#include <queue>
#include <unordered_map>
#include "can_matrix_protocol.hpp"

using namespace std;

namespace AutoDataService
{
    // this module name
    extern const string SOC_AUTO_DATA_SERVICE_MANAGER_NAME;

    // typedef struct _FuncArray {
    //     uint32_t can_id;
    //     void(*processHandle)(const can_frame&);
    // }CanDataProcessArray;

    typedef void(*processHandle)(const can_frame&);

    class ServiceManager : public AppFw::Module, public ServiceInterface
    {
    public:
        ServiceManager();
        virtual ~ServiceManager();
    private:
        /**
        * @fn onProcess
        * @brief main thread process function
        * @return void
        */
        virtual void onProcess();

        void canDataHandler();
    private:
        /**
        * @fn proess_vcu_vehinfoa
        * @brief process IC message.
        * @return void
        */
        void process_vcu_vehinfoa(const can_frame& frame);
        /**
        * @fn proess_vcu_vehinfob
        * @brief process IC message.
        * @return void
        */
        void process_vcu_vehinfob(const can_frame& frame);

        /**
        * @fn proess_vcu_motst
        * @brief process IC message.
        * @return void
        */
        void process_vcu_motst(const can_frame& frame);

        /**
        * @fn proess_vcu_ccvs
        * @brief process IC message.
        * @return void
        */
        void process_vcu_ccvs(const can_frame& frame);

        /**
        * @fn proess_vcu_ccvs
        * @brief process IC message.
        * @return void
        */
        void process_eps_error(const can_frame& frame);

        /**
        * @fn proess_vcu_dispinfo
        * @brief process IC message.
        * @return void
        */
        void process_vcu_dispinfo(const can_frame& frame);

        /**
        * @fn proess_vcu_cpd1
        * @brief process IC message.
        * @return void
        */
        void process_vcu_cpd1(const can_frame& frame);

        /**
        * @fn proess_vcu_cpd3
        * @brief process IC message.
        * @return void
        */
        void process_vcu_cpd3(const can_frame& frame);

        /**
        * @fn proess_vcu_configstatus
        * @brief process IC message.
        * @return void
        */
        void process_vcu_configstatus(const can_frame& frame);

        /**
        * @fn receive
        * @brief receive message and put into cycle buffer
        * @return void
        */
        void receive();

        /**
        * @fn readMsg
        * @brief receive message and put into cycle buffer
        * @return bool true is succuss , or not.
        */
        bool readMsg(can_frame& frame);
    private:
        DomainService           m_domainService;
        GaugeService            m_gaugeService;
    private:
        std::condition_variable m_recv_condition;
        std::thread             m_recv_thread;
        std::mutex              m_recv_mutex;
        bool                    m_recv_stop;
        std::queue<can_frame>   m_frameBuffer;
        std::mutex              m_data_mutex;
    private:
        std::unordered_map<int, std::function<void(const can_frame&)>> m_funcMap;
        std::unordered_map<int, can_frame> m_mapCanframe;
        atomic<uint> m_speedInterval ;
        atomic<uint> m_motorPowerRateInterval ;
    };
}
