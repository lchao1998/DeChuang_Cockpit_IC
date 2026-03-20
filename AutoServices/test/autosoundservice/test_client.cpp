#include "soundclient/client_vsomeip.hpp"

#include <gtest/gtest.h>
#include <thread>

using namespace AutoSoundService;

TEST(SoundClient, VSomeipClient)
{
    VSomeipClient client(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);

    if(client.init())
    {
        client.start();
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (true)
    {
        client.send(std::vector<uint8_t>{1, 2, 3});
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main (int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
