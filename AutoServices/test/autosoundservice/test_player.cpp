#include "player/pcm.hpp"
#include "player/wav_player.hpp"
#include "resourcemanager/resource_manager.hpp"

#include <queue>
#include <thread>
#include <gtest/gtest.h>

#define LOAD_PARAMS(_soundType) \
PlayParams{ .soundType              = _soundType,                                       \
            .playerType             = PlayerType::WAV,                                  \
            .playType               = PlayType::TIMES,                                  \
            .intervalMs             = ResourceManager::GetAudioFilePath(static_cast<SoundType>(_soundType)).interval_ms,  \
            .repeatTimesOrDuration  = 0,                                                \
            .fileName               = ResourceManager::SoundType2String(_soundType),    \
            .audioPaths             = ResourceManager::GetAudioFilePath(static_cast<SoundType>(_soundType)).pathContainer \
}

#define PLAY_PARAMS(_soundType, _playType, _deadTimeMs, _repeatTimesOrDuration)         \
PlayParams{ .soundType              = _soundType,                                       \
            .playerType             = PlayerType::WAV,                                  \
            .playType               = _playType,                                        \
            .intervalMs             = _deadTimeMs,                                      \
            .repeatTimesOrDuration  = _repeatTimesOrDuration,                           \
            .fileName               = ResourceManager::SoundType2String(_soundType),    \
            .audioPaths             = ResourceManager::GetAudioFilePath(static_cast<SoundType>(_soundType)).pathContainer \
}

using namespace AutoSoundService;

int RunCmd(const std::string& cmd, std::string& out) {
    FILE* fp = popen(cmd.c_str(), "r");
    if (!fp) {
        return errno == 0 ? -1 : errno;
    }

    char buffer[4096] = { 0 };
    while(!feof(fp)){
        size_t len = fread(buffer, 1, 4096, fp);
        if(len > 0) out.append(buffer, len);
    }
    out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());
    return pclose(fp);
}

TEST(Player, WaveFile)
{
    WaveFile waveFile;

    /* 1. test load audio file form file path */
    EXPECT_FALSE(waveFile.load(nullptr));
    EXPECT_FALSE(waveFile.load("null.wav"));
    EXPECT_TRUE(waveFile.load("6_Channel_ID.wav"));
    EXPECT_TRUE(waveFile.load("6_Channel_ID.wav"));
    EXPECT_TRUE(waveFile.load("6_Channel_ID.wav"));
    EXPECT_FALSE(waveFile.load("null"));

    /* 2. show file info */
    printf("%s\n", waveFile.info().c_str());

    /* 3. test file info */
    EXPECT_EQ(6, waveFile.channels());
    EXPECT_EQ(16, waveFile.bitPerSample());
    EXPECT_EQ(44100, waveFile.sampleRate());
    EXPECT_EQ(529200, waveFile.byteRate());
    EXPECT_EQ(4233600, waveFile.bitRate());
    EXPECT_EQ(257411, waveFile.frames());
    EXPECT_EQ(3088932, waveFile.dataLength());
    EXPECT_NE(nullptr, waveFile.data());

    /* 4. test load audio files from AudioFilePath */
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        AudioFiles tmp = ResourceManager::GetAudioFilePath(static_cast<SoundType>(i));
        EXPECT_TRUE(waveFile.load(tmp.pathContainer, tmp.interval_ms));
    }

    /* 5. test file info */
    AudioFiles audioFiles = ResourceManager::GetAudioFilePath(static_cast<SoundType>(SoundType::BUZZER_1));
    EXPECT_TRUE(waveFile.load(audioFiles.pathContainer, audioFiles.interval_ms));
    printf("%s\n", waveFile.info().c_str());
    EXPECT_EQ(2, waveFile.channels());
    EXPECT_EQ(16, waveFile.bitPerSample());
    EXPECT_EQ(48000, waveFile.sampleRate());
    EXPECT_EQ(192000, waveFile.byteRate());
    EXPECT_EQ(1536000, waveFile.bitRate());
    EXPECT_EQ(6711, waveFile.frames());
    EXPECT_EQ(192000, waveFile.dataLength());
    EXPECT_NE(nullptr, waveFile.data());

    /* 6. test load audio files from SoundType */
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        AudioFiles tmp = ResourceManager::GetAudioFilePath(static_cast<SoundType>(i));
        EXPECT_TRUE(waveFile.load(tmp.pathContainer, tmp.interval_ms));
    }

    /* 7. test file info */
    audioFiles = ResourceManager::GetAudioFilePath(static_cast<SoundType>(SoundType::BUZZER_1));
    EXPECT_TRUE(waveFile.load(audioFiles.pathContainer, audioFiles.interval_ms));
    printf("%s\n", waveFile.info().c_str());
    EXPECT_EQ(2, waveFile.channels());
    EXPECT_EQ(16, waveFile.bitPerSample());
    EXPECT_EQ(48000, waveFile.sampleRate());
    EXPECT_EQ(192000, waveFile.byteRate());
    EXPECT_EQ(1536000, waveFile.bitRate());
    EXPECT_EQ(6711, waveFile.frames());
    EXPECT_EQ(192000, waveFile.dataLength());
    EXPECT_NE(nullptr, waveFile.data());

    /* 8. test clear file*/
    EXPECT_FALSE(waveFile.load("null.wav"));
    waveFile.clear();
    EXPECT_EQ(0, waveFile.channels());
    EXPECT_EQ(0, waveFile.bitPerSample());
    EXPECT_EQ(0, waveFile.sampleRate());
    EXPECT_EQ(0, waveFile.byteRate());
    EXPECT_EQ(0, waveFile.bitRate());
    EXPECT_EQ(0, waveFile.frames());
    EXPECT_EQ(0, waveFile.dataLength());
    EXPECT_EQ(nullptr, waveFile.data());
}

TEST(Player, WavContainer)
{
    WavContainer wavContainer;
    EXPECT_TRUE(wavContainer.empty());

    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_5));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_5));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_5));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_1));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_1));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_1));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_1));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_14));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_14));
    wavContainer.add(LOAD_PARAMS(SoundType::BUZZER_14));

    EXPECT_EQ(3, wavContainer.size());

    EXPECT_EQ(2,        wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_5))->channels());
    EXPECT_EQ(16,       wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_5))->bitPerSample());
    EXPECT_EQ(48000,    wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_5))->sampleRate());

    EXPECT_EQ(2,        wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_1))->channels());
    EXPECT_EQ(16,       wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_1))->bitPerSample());
    EXPECT_EQ(48000,    wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_1))->sampleRate());

    EXPECT_EQ(2 ,       wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_14))->channels());
    EXPECT_EQ(16,       wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_14))->bitPerSample());
    EXPECT_EQ(48000,    wavContainer.get(LOAD_PARAMS(SoundType::BUZZER_14))->sampleRate());

    EXPECT_FALSE(wavContainer.empty());
    wavContainer.clear();
    EXPECT_EQ(0 , wavContainer.size());
    EXPECT_TRUE(wavContainer.empty());
}

TEST(Player, WavPlayer)
{
    WavPlayer wavPlayer([=](const SoundState &state) -> void {
        EXPECT_STREQ("/usr/local/audios/BUZZER-2.wav, 0ms", state.name.c_str());
        EXPECT_EQ(static_cast<uint32_t>(SoundType::BUZZER_1), state.priority);
        EXPECT_EQ(PlayerType::WAV, state.playerType);
        EXPECT_EQ(PlayType::TIMES, state.type);

        if (PlayState::PLAYING == state.state)
        {
            EXPECT_EQ(10, state.remainTimesOrDuration);
        }
        else if (PlayState::IDLE == state.state)
        {
            EXPECT_EQ(0, state.remainTimesOrDuration);
        }
        else
        {
            EXPECT_TRUE(false);
        }
    });

    wavPlayer.play(PLAY_PARAMS(SoundType::BUZZER_2, PlayType::TIMES, 0, 10));

    WavPlayer wavPlayer2([=](const SoundState &state) -> void {
        EXPECT_STREQ("/usr/local/audios/BUZZER-1_Deng.wav, /usr/local/audios/BUZZER-1_Dong.wav, 500ms", state.name.c_str());
        EXPECT_EQ(static_cast<uint32_t>(SoundType::BUZZER_1), state.priority);
        EXPECT_EQ(PlayerType::WAV, state.playerType);
        EXPECT_EQ(PlayType::REPEAT, state.type);

        std::string ALSAState;
        if (PlayState::PLAYING == state.state)
        {
            EXPECT_EQ(0, state.remainTimesOrDuration);
            EXPECT_EQ(0, RunCmd("cat /proc/asound/card0/pcm0p/sub0/status", ALSAState));
            EXPECT_STRNE("closed", ALSAState.c_str());
        }
        else if (PlayState::IDLE == state.state)
        {
            EXPECT_EQ(65535, state.remainTimesOrDuration);
            PCM::Instance().close();
            EXPECT_EQ(0, RunCmd("cat /proc/asound/card0/pcm0p/sub0/status", ALSAState));
            EXPECT_STREQ("closed", ALSAState.c_str());
        }
        else
        {
            EXPECT_TRUE(false);
        }
    });

    std::thread td([&](){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        wavPlayer2.stop(StopType::IMMEDIATELY);
    });
    wavPlayer2.play(PLAY_PARAMS(SoundType::BUZZER_1, PlayType::REPEAT, 500, 0));
    td.join();
}

int main (int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
