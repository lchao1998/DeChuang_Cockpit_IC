#include "socketcan_controller.hpp"
#include <string.h>
namespace AutoDataService
{
    int32_t SocketCanController::init(const char* device){
        if (device == nullptr) return -1;
        struct ifreq ifr;
        m_socket_can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (m_socket_can_fd < 0) return -2;
        strcpy(ifr.ifr_name, device);

        ioctl(m_socket_can_fd, SIOCGIFINDEX, &ifr); 
        m_addr_can.can_family = AF_CAN;
        m_addr_can.can_ifindex = ifr.ifr_ifindex;
        bind(m_socket_can_fd, (struct sockaddr *)&m_addr_can, sizeof(m_addr_can)); 

        if (m_pfilter) {
            setsockopt(m_socket_can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &m_pfilter, sizeof(m_pfilter));
        }
        int canfd_on = 1;
        setsockopt(m_socket_can_fd, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &canfd_on, sizeof(canfd_on));
        m_bInited = true;
        return 0;
    }

    int SocketCanController::setFilter(can_filter *filter, int size){
        if (size <= 0) return -1;

        m_pfilter = new can_filter[size];
        if (!m_pfilter) return -2;
        for (size_t i = 0; i < size; i++)
        {
            m_pfilter[i] = filter[i];
        }
        return 0;
    }

    int32_t SocketCanController::send(can_frame& frame, int32_t len){
        if (m_socket_can_fd <= 0) return -1;
        
        write(m_socket_can_fd, &frame, len);
    }

    int32_t SocketCanController::recv(can_frame& status){
        if (m_socket_can_fd <= 0) return -1;

        read(m_socket_can_fd, &status, sizeof(can_frame));
    }

    void SocketCanController::unInit(){
        m_bInited = false;
        if (m_pfilter){
            delete[] m_pfilter;
            m_pfilter = nullptr;
        }
    }

    bool SocketCanController::bInited() {
        return m_bInited;
    }
} // namespace AutoDataService
