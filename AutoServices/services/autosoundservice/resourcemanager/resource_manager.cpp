#include "resource_manager.hpp"
#include "Log/log.hpp"

#include <algorithm>

namespace AutoSoundService
{
    constexpr char AudioDir[] = "/usr/local/audios/";

    std::map<SoundType,  AudioFiles> ResourceManager::SoundFileMaps = {         // NOLINT
            {SoundType::BUZZER_1,   {{"BUZZER-1_Deng.wav", "BUZZER-1_Dong.wav"},    500}},
            {SoundType::BUZZER_20,  {{"BUZZER-20.wav"},                             0}},
            {SoundType::BUZZER_38,  {{"BUZZER-38.wav"},                             0}},
            {SoundType::BUZZER_2,   {{"BUZZER-2.wav"},                              0}},
            {SoundType::BUZZER_5,   {{"BUZZER-5.wav"},                              0}},
            {SoundType::BUZZER_27,  {{"BUZZER-27.wav"},                             0}},
            {SoundType::BUZZER_14,  {{"BUZZER-14.wav"},                             0}},
            {SoundType::BUZZER_47,  {{"BUZZER-47.wav"},                             500}},
    };

    AudioFiles ResourceManager::GetAudioFilePath(const SoundType &soundType)
    {
        AudioFiles res;
        auto iterator = SoundFileMaps.find(soundType);

        if ( SoundFileMaps.end() != iterator)
        {
            for (auto &item: iterator->second.pathContainer)
            {
                res.pathContainer.push_back(AudioDir + item);
            }
            res.interval_ms = iterator->second.interval_ms;
        }

        return res;
    }

    PlayerType ResourceManager::GetPlayerType(const SoundType &soundType) {
        std::string fileName;
        PlayerType res = PlayerType::Size;

        auto iterator = SoundFileMaps.find(soundType);
        if (SoundFileMaps.end() != iterator)
        {
            fileName = iterator->second.pathContainer[0];
            std::string suffix = fileName.substr(fileName.find_last_of('.') + 1);
            std::transform(suffix.begin(),suffix.end(),suffix.begin(), ::tolower);

            if (suffix == "wav")
            {
                res = PlayerType::WAV;
            }
            else
            {
                res = PlayerType::Size;
                logWarning("Unsupported format: %s", suffix.c_str());
            }
        }

        return res;
    }

    std::string ResourceManager::GetPlayerName(const PlayerType &playerType) {
        std::string ret;
        switch (playerType)
        {
            case PlayerType::WAV :
                ret = "WavPlayer";
                break;
            case PlayerType::Size:
                ret = "None";
                break;
            default:
                break;
        }
        return ret;
    }

    std::string ResourceManager::SoundType2String(const SoundType &soundType) {
        std::string res;
        AudioFiles files = ResourceManager::GetAudioFilePath(soundType);
        for (auto &item: files.pathContainer)
        {
            res += (item + ", ");
        }
        res += std::to_string(files.interval_ms) + "ms";
        return res;
    }
}
