#include "Log/log.hpp"
#include "sound_manager.hpp"
#include "soundservices/sound_service.hpp"
#include "controller/player_controller.hpp"

using std::string;

namespace AutoSoundService
{
    const std::string SOC_SOUND_SERVICE_MANAGER_NAME{"SoundManager"}; // NOLINT

    struct SoundManagerPrivate
    {
        SoundService srv;
        PlayerController playerController;
    };

    SoundManager::SoundManager()
            : m_p(new SoundManagerPrivate)
    {
        logDebug("[SoundManager::SoundManager]: begin to construct SoundManager");
        m_p->srv.registerHandler("reqPlaySnd", std::function<void(SoundType, PlayType, uint16_t, uint16_t)>(
                [this](SoundType soundType, PlayType type, uint16_t intervalMs, uint16_t repeatTimesOrDuration) -> void
                {
                    this->reqPlaySnd(soundType, type, intervalMs, repeatTimesOrDuration);
                }));

        m_p->srv.registerHandler("reqStopSnd", std::function<void(StopType, SoundType)>(
                [this](StopType stopType, SoundType soundType) -> void
                {
                    this->reqStopSnd(stopType, soundType);
                }));

        m_p->srv.registerHandler("reqReset", std::function<void(void)>(
                [this]() -> void
                {
                    this->reqReset();
                }));

        m_p->playerController.start();
        logDebug("[SoundManager::SoundManager]: construct SoundManager finished");
    }

    SoundManager::~SoundManager()
    {
        logDebug("[SoundManager::~SoundManager]: player controller going to stop");
        m_p->playerController.stop();
        logDebug("[SoundManager::~SoundManager]: player controller stop finished!");
    }

    void SoundManager::reqPlaySnd(const SoundType &soundType, const PlayType &playType,
                                  uint16_t deadTimeMs, uint16_t repeatTimesOrDuration)
    {
        PlayParams params;
        params.soundType = soundType;
        params.playType = playType;
        params.intervalMs = deadTimeMs;
        params.repeatTimesOrDuration = repeatTimesOrDuration;

        logDebug("[SoundManager::reqPlaySnd]: name: %d, type: %d, repeatTimesOrDuration: %d, deadTimeMs: %d",
                 (int)params.soundType, (int)params.playType, params.repeatTimesOrDuration, params.intervalMs);

        this->postTask([=]{ this->onReqPlaySnd(params); });
    }

    void SoundManager::reqStopSnd(const StopType &stopType, const SoundType &soundType)
    {
        logDebug("[SoundManager::reqStopSnd]: stopType: %d, soundType: %d", static_cast<int>(stopType), static_cast<int>(soundType));
        StopParams params{stopType, soundType};
        this->postTask([=]{ this->onReqStopSnd(params); });
    }

    void SoundManager::reqReset()
    {
        logDebug("[SoundManager::reqReset]: handle reqReset");
        this->postTask([=]{ this->onReqReset(); });
    }

    void SoundManager::onReqPlaySnd(const PlayParams& params)
    {
        logDebug("SoundManager::onReqPlaySnd");
        m_p->playerController.addTask(params);
    }

    void SoundManager::onReqStopSnd(const StopParams& params)
    {
        logDebug("SoundManager::onReqStopSnd");
        m_p->playerController.removeTask(params);
    }

    void SoundManager::onReqReset()
    {
        logDebug("SoundManager::onReqReset");
        m_p->playerController.reset();
    }
}