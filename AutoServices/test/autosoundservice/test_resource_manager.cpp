#include "resourcemanager/resource_manager.hpp"
#include "AutoServices/services/autosoundservice/utils/type_define.hpp"

#include <map>
#include <fstream>
#include <gtest/gtest.h>

#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_MGT "\033[0;35m"   //Magenta
#define ANSI_TXT_DFT "\033[0;0m"    //Console default
#define GTEST_BOX "[     INFO ] "
#define COUT_GTEST ANSI_TXT_GRN << GTEST_BOX //You could add the Default
#define COUT_GTEST_MGT COUT_GTEST << ANSI_TXT_MGT

using namespace AutoSoundService;

TEST(ResourceManager, SoundType2String)
{
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        std::string filePaths = ResourceManager::SoundType2String(static_cast<SoundType>(i));
        EXPECT_NE(filePaths.size(), 0);
        std::cout << COUT_GTEST_MGT << "SoundType : " << i << " = " << filePaths << ANSI_TXT_DFT << std::endl;
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-redundant-string-cstr"
TEST(ResourceManager, GetAudioFilePath)
{
    // 1. check if it has the all sound type support
    EXPECT_EQ(ResourceManager::Size(), static_cast<size_t>(SoundType::SIZE));

    // 2. check if it has the same element
    std::set<std::string> audioFileList;
    std::pair<std::set<std::string>::iterator, bool> element;
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        AudioFiles audioFile = ResourceManager::GetAudioFilePath(static_cast<SoundType>(i));
        for (auto &item : audioFile.pathContainer) {
            element = audioFileList.insert(item);
            EXPECT_TRUE(element.second);
        }

    }

    // 3. check file if exists
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        AudioFiles audioFile = ResourceManager::GetAudioFilePath(static_cast<SoundType>(i));
        for (auto &item : audioFile.pathContainer) {
            std::ifstream file(item.c_str());
            EXPECT_TRUE(file.good());
        }
    }
}
#pragma clang diagnostic pop

TEST(ResourceManager, GetPlayerType)
{
    for (int i = 0; static_cast<SoundType>(i) < SoundType::SIZE; ++i)
    {
        EXPECT_EQ(PlayerType::WAV, ResourceManager::GetPlayerType(static_cast<SoundType>(i)));
    }
    EXPECT_EQ(PlayerType::Size, ResourceManager::GetPlayerType(SoundType::SIZE));
}

TEST(ResourceManager, GetPlayerName)
{
    EXPECT_STREQ("WavPlayer", ResourceManager::GetPlayerName(PlayerType::WAV).c_str());
    EXPECT_STREQ("None", ResourceManager::GetPlayerName(PlayerType::Size).c_str());
}

int main (int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
