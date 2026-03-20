/**
* @file         client_vsomeip.hpp
* @brief
* @details
* @author       long.xinzheng@zlingsmart.com
* @date         2022-8-9
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention
*
* @par modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/08/09  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#ifndef ZL_XH_2022002_IC_CLIENT_VSOMEIP_HPP
#define ZL_XH_2022002_IC_CLIENT_VSOMEIP_HPP

#include <memory>
#include <vsomeip/vsomeip.hpp>

namespace AutoSoundService
{
#define SAMPLE_SERVICE_ID       0x1234
#define SAMPLE_INSTANCE_ID      0x5678
#define SAMPLE_METHOD_ID        0x0421
    class VSomeipClient final
    {
    public:
        VSomeipClient() = delete;
        VSomeipClient(const VSomeipClient &) = delete;
        VSomeipClient & operator = (const VSomeipClient &) = delete;

        explicit VSomeipClient(uint16_t serviceId = SAMPLE_SERVICE_ID, uint16_t instanceId = SAMPLE_INSTANCE_ID, uint16_t methodId = SAMPLE_METHOD_ID);
        ~VSomeipClient();

        bool init();

        void start() ;

        void stop();

        bool send(const std::vector<uint8_t> &data);

    private:
        void onState(vsomeip::state_type_e state);

        void onMessage(const std::shared_ptr<vsomeip::message> &response);

        void onAvailability(vsomeip::service_t service, vsomeip::instance_t instance, bool isAvailable);

    private:
        struct VSomeipClientPrivate *m_p;
    };
}

#endif //ZL_XH_2022002_IC_CLIENT_VSOMEIP_HPP
