
/**
* @file         socketcan_controller.hpp
* @brief        deal with socket recv and send data from can bus.
* @details      A Singleton controller socket class
* @author       xu.qiang@zlingsmart.com
* @date         2022-11-1
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention
*
* @par modity log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/06/15  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#pragma once 

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace AutoDataService
{
    #define ip_cmd_set_can0_params "ip link set can0 type can bitrate 5000000 triple-sampling on"
    #define ip_cmd_can0_up         "ifconfig can0 up"
    #define ip_cmd_can0_down       "ifconfig can0 down"
    #define default_can_device     "can0"
    // 使用系统调用函数运行以上命令，也可以自行在终端中运行
    //system(ip_cmd_set_can0_params); // 设置参数
    //system(ip_cmd_can0_up);  // 开启can0接口

	class SocketCanController
	{
	public:
		static SocketCanController& getInstance() {
			static SocketCanController instance;
			return instance;
		}

	public:
		/**
		* @fn bInited.
		* @brief init clinet socket is ok?.
		* @return bool : true is inited,false is uninited.
		*/
		bool bInited();  
		/**
		* @fn init.
        * @param char* device : can device 
		* @brief init socket can .
		* @return int32_t :  0 is success ,other is failed.
		*/
		int32_t init(const char* device = default_can_device);

        /**
		* @fn setFilter.
        * @param can_filter *filter : can filter array .
        * @param int size: filter array size.
		* @brief init socket can .
		* @return set status , 0 is success ,other is failed.
		*/
        int setFilter(can_filter *filter, int size);
		/**
		* @fn uninit.
		* @brief uninit socket.
		* @return void.
		*/
		void unInit();

		/**
		* @fn send.
		* @brief send buffer info to can bus.
		* @param can_frame& frame : send data.
		* @param int32_t len : send data length.
		* @return int : sended length.
		*/
		int32_t send(can_frame& frame, int32_t len);

		/**
		* @fn recv.
		* @brief recv buffer info from server.
		* @param uint8_t* pBuffer : recv data.
		* @return int : received data length.
		*/
		int32_t recv(can_frame& status);
	private:
		int32_t m_socket_can_fd;
		struct sockaddr_can  m_addr_can;
		bool m_bInited = false;
        struct can_filter *m_pfilter = nullptr;
	private:
		SocketCanController() {};
		SocketCanController(const SocketCanController& sockt) = delete;
		SocketCanController& operator=(const SocketCanController& Sock) = delete;
		~SocketCanController() { unInit(); };
	};
}