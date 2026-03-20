#include "sound_service.hpp"
#include "service_define.hpp"
#include "utils/type_define.hpp"

namespace AutoSoundService
{

    SoundService::SoundService()
        : ServiceBase(IPC_SERVICE_ID, IPC_SERVICE_INSTANCE_SOUND, IPC_Service_Name_Sound)
    {
        m_service->setInvokeMethodHandler("reqPlaySnd", [this](const AutoSOCM::Data& parameter, const AutoSOCM::Service::MethodReturnHandler& returnHandler, uint16_t client){
            (void)client;

            SoundType soundType;
            PlayType playType;
            uint16_t intervalMs = 0;
            uint16_t repeatTimesOrDuration = 0;
            AutoSOCM::DataArray arr;
            try
            {
                arr = parameter.toArray();
                soundType               = static_cast<SoundType>(arr[0].toUInt());
                playType                = static_cast<PlayType>(arr[1].toUInt());
                intervalMs              = arr[2].toUInt();
                repeatTimesOrDuration   = arr[3].toUInt();
            }
            catch (const std::exception &e)
            {
                logError("[SoundService] parse \"reqPlaySnd\" params failed, parameter.size() = %zu, what: %s", arr.size(), e.what());
                return returnHandler(parameter);
            }

            executeHandler<void(SoundType, PlayType, uint16_t, uint16_t)>(
                    "reqPlaySnd",
                    soundType,
                    playType,
                    intervalMs,
                    repeatTimesOrDuration
            );
            return returnHandler(parameter);
        });

        m_service->setInvokeMethodHandler("reqStopSnd", [this](const AutoSOCM::Data& parameter, const AutoSOCM::Service::MethodReturnHandler& returnHandler, uint16_t client){
            (void)client;
            StopType stopType;
            SoundType soundType;
            AutoSOCM::DataArray arr;
            try
            {
                arr = parameter.toArray();
                stopType  = static_cast<StopType>(arr[0].toUInt());
                soundType = static_cast<SoundType>(arr[1].toUInt());
            }
            catch (std::exception &e)
            {
                logError("[SoundService] catch exception while call parameter.toUInt(), parameter size: %zu, what: %s", arr.size(), e.what());
                return returnHandler(parameter);
            }
            executeHandler<void(StopType, SoundType)>("reqStopSnd", stopType, soundType);
            return returnHandler(parameter);
        });

        m_service->setInvokeMethodHandler("reqReset", [this](const AutoSOCM::Data& parameter, const AutoSOCM::Service::MethodReturnHandler &returnHandler, uint16_t client){
            (void)client;
            executeHandler<void(void)>("reqReset");
            return returnHandler(parameter);
        });
    }

    void SoundService::registerHandler(const std::string &name, const boost::any& func)
    {
        auto iter = m_handlers.find(name);
        if(iter == m_handlers.end())
        {
            m_handlers[name] = func;
        }
    }
}