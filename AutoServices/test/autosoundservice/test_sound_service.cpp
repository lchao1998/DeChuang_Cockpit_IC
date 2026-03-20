#include "AutoSOCM/include/application.hpp"
#include "soundservices/service_define.hpp"
#include "include/auto_sound_service.hpp"

#include <gtest/gtest.h>
#include <thread>

#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_MGT "\033[0;35m"   //Magenta
#define ANSI_TXT_DFT "\033[0;0m"    //Console default
#define GTEST_BOX "[     INFO ] "
#define COUT_GTEST ANSI_TXT_GRN << GTEST_BOX //You could add the Default
#define COUT_GTEST_MGT COUT_GTEST << ANSI_TXT_MGT

#define INVOKE_PLAY_SND(client, soundType, playType, deadTimeMs, repeatTimesOrDuration) \
do                                                                                      \
{                                                                                       \
client->invokeMethodAsync("reqPlaySnd",  GetPlayParams(soundType, playType, deadTimeMs, repeatTimesOrDuration), \
                            [&](const AutoSOCM::Data& returnValue){                 \
                                AutoSOCM::DataArray arr = returnValue.toArray();    \
                                std::cout << "Method \"reqPlaySnd\" returned: ";    \
                                for(auto & i : arr)                                 \
                                {                                                   \
                                    std::cout << i.toString() << " ";               \
                                }                                                   \
                                std::cout << std::endl;                             \
});                                                                                 \
}while(0)

#define INVOKE_STOP_SND(client, stopType, soundType)                                \
do                                                                                  \
{                                                                                   \
client->invokeMethodAsync("reqStopSnd",  GetStopParams(stopType, soundType),        \
                            [&](const AutoSOCM::Data& returnValue){                 \
                                AutoSOCM::DataArray arr = returnValue.toArray();    \
                                std::cout << "Method \"reqStopSnd\" returned: ";    \
                                for(auto & i : arr)                                 \
                                {                                                   \
                                    std::cout << i.toString() << " ";               \
                                }                                                   \
                                std::cout << std::endl;                             \
});                                                                                 \
}while(0)

#define INVOKE_RST(client)                                                          \
do                                                                                  \
{                                                                                   \
client->invokeMethodAsync("reqReset", [&](const AutoSOCM::Data& returnValue){       \
                              std::cout << "Method \"reqStopSnd\" returned: ";      \
                              if (returnValue.empty())                              \
                              {                                                     \
                                  std::cout << std::endl;                           \
                                  return;                                           \
                              }                                                     \
                              AutoSOCM::DataArray arr = returnValue.toArray();      \
                              for(auto & i : arr)                                   \
                              {                                                     \
                                  std::cout << i.toString() << " ";                 \
                              }                                                     \
                              std::cout << std::endl;                               \
                          });                                                       \
}while(0)

using namespace AutoSoundService;

TEST(AutoSoundService, reqPlaySnd)
{
    unsigned int interval = 10;
    std::shared_ptr<AutoSOCM::Application> app = AutoSOCM::getApplication();
    std::shared_ptr<AutoSOCM::Client> client = app->createClient(AutoSoundService::IPC_SERVICE_ID, AutoSoundService::IPC_SERVICE_INSTANCE_SOUND);

    uint durationTime = 5; // 5s
    uint count = 0;
    client->setAvailabilityChangedHandler([&](bool availability){
        if(availability)
        {
            // Reset
            INVOKE_RST(client);
            std::this_thread::sleep_for(std::chrono::seconds(1));

            // BUZZER_47 > BUZZER_14(repeat) > BUZZER_27 > BUZZER_5 > BUZZER_2 > BUZZER_38 > BUZZER_20(repeat) > BUZZER_1(repeat)
            std::cout << COUT_GTEST_MGT << "-----------------> Test Begin <-----------------" << ANSI_TXT_DFT << std::endl;
            /* 1. send high priority repeat SoundType, while playing low priority repeat sound */
            /*    EXPECT: play high priority repeat sound -> play end -> play low priority repeat sound -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_1 -> BUZZER_5 -> BUZZER_1 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 2. send high priority non-repeat SoundType, while playing low priority repeat sound */
            /*    EXPECT: play high priority non-repeat sound -> play end -> play low priority repeat sound -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_1 -> BUZZER_5 -> BUZZER_1 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::REPEAT, 500, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 3. send high priority repeat SoundType, while playing low priority non-repeat sound */
            /*    EXPECT: play high priority repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_1 -> BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::DURATION, 500, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 4. send high priority non-repeat SoundType, while playing low priority non-repeat sound */
            /*    EXPECT: play high priority non-repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_1 -> BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::DURATION, 500, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 2);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 5. send low priority repeat SoundType, while playing high priority repeat sound */
            /*    EXPECT: play high priority repeat sound -> play low priority repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> BUZZER_1 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::REPEAT, 500, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 6. send low priority non-repeat SoundType, while playing high priority repeat sound */
            /*    EXPECT: play high priority repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::DURATION, 500, 2);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 7. send low priority repeat SoundType, while playing high priority non-repeat sound */
            /*    EXPECT: play high priority non-repeat sound -> play low priority repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> BUZZER_1 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::REPEAT, 500, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 8. send low priority non-repeat SoundType, while playing high priority non-repeat sound */
            /*    EXPECT: play high priority non-repeat sound -> play end -> stop */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::DURATION, 500, 2);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 9. send the same priority repeat SoundType, while playing non-repeat sound */
            /*    EXPECT: drop request */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 10. send the same priority non-repeat SoundType, while playing non-repeat sound */
            /*    EXPECT: drop request */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 50);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 2);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 11. send the same priority non-repeat SoundType, while playing repeat sound */
            /*    EXPECT: drop request */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::DURATION, 0, 2);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 12. send the same priority repeat SoundType, while playing repeat sound */
            /*    EXPECT: drop request */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 13. send multi different priority repeat SoundType */
            /*    EXPECT: drop request */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_1 -> BUZZER_20 -> BUZZER_5 -> BUZZER_14 -> BUZZER_5 -> BUZZER_20 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_1, PlayType::REPEAT, 500, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_20, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_5, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_PLAY_SND(client, SoundType::BUZZER_14, PlayType::REPEAT, 0, 0);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));

            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_1);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_14);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_5);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            INVOKE_STOP_SND(client, StopType::FINISH_CYCLE, SoundType::BUZZER_20);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            /* 14. test DURATION play type */
            /*    EXPECT: BUZZER_47 x 5 */
            std::cout << COUT_GTEST_MGT << "Run test case " << ++count << " ..." << ANSI_TXT_DFT << std::endl;
            std::cout << COUT_GTEST_MGT << "EXPECT: BUZZER_47 x 5 -> stop" << ANSI_TXT_DFT << std::endl;
            INVOKE_PLAY_SND(client, SoundType::BUZZER_47, PlayType::DURATION, 0, 5);
            std::this_thread::sleep_for(std::chrono::seconds(durationTime));
            std::cout << COUT_GTEST_MGT << "Run test case " << count << " ... OK" << ANSI_TXT_DFT << std::endl;

            std::cout << COUT_GTEST_MGT << "-----------------> Test End <-----------------" << ANSI_TXT_DFT << std::endl;
        }
    });

    client->setPollable(true);

    app->startAsync();
    while(app->isRunning())
    {
        client->poll();
        if (count >= 14) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
    app->stop();
 }

int main (int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
