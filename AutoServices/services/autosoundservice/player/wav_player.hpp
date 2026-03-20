/**
* @file         wav_player.hpp
* @brief
* @details
* @author       long.xinzheng@zlingsmart.com
* @date         2022-6-9
* @version      v0.0.1
* @copyright    Copyright (c) 2022, zlingsmart Corporation.
**********************************************************************************
* @attention
*
* @par modify log:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2022/02/21  <td>v0.0.1   <td>          <td>Create
* </table>
*
**********************************************************************************
*/
#ifndef AUTOSERVICES_WAV_PLAYER_HPP
#define AUTOSERVICES_WAV_PLAYER_HPP

#include "player_base.hpp"

namespace AutoSoundService
{
    struct AudioFiles;
    class WaveFile final
    {
    public:
        explicit WaveFile();

        ~WaveFile();

        bool load(const char *fileName);

        bool load(const std::vector<std::string> &files, uint32_t interval_ms = 0);

        void clear();

        u_char* data() const;

        uint32_t dataLength() const;

        uint32_t frames() const;

        uint32_t bitPerSample() const;

        uint32_t sampleRate() const;

        uint32_t byteRate() const;

        uint32_t bitRate() const;

        uint32_t channels() const;

        uint32_t duration() const;

        uint32_t interval() const;

        std::string info() const;

        typedef std::shared_ptr<WaveFile> Ptr;

    private:
        template<typename ... Args>
        static std::string StringFormat( const std::string& format, Args ... args)
        {
            int size_s = std::snprintf( nullptr, 0, format.c_str(), args ...) + 1; // add '\0'
            if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
            auto size = static_cast<size_t>( size_s );
            std::unique_ptr<char[]> buf(new char[ size ]);
            std::snprintf(buf.get(), size, format.c_str(), args ...);
            return std::string{buf.get(), buf.get() + size - 1}; // remove '\0'
        }

    private:
        std::unique_ptr<struct WaveFilePrivate> m_p;
    };

    class WavContainer final
    {
    public:
        explicit WavContainer();
        ~WavContainer();

        WaveFile::Ptr get(const PlayParams &playParams);
        void add(const PlayParams &playParams);

        size_t size();

        bool empty();

        void clear();

        typedef std::shared_ptr<WavContainer> Ptr;
    private:
        std::unique_ptr<struct WavContainerPrivate> m_p;
    };

    class WavPlayer final : public PlayerBase
    {
    public:
        explicit WavPlayer(const std::function<void(const SoundState &)> &cb = nullptr);

        ~WavPlayer() override;
        void play(const PlayParams& params) override;
        void stop(const StopType& stopType) override;

    private:

        void playWorker(const PlayParams& playParams);

        static uint16_t getRepeatTimes(const WaveFile::Ptr &wavPtr, PlayType playType, uint16_t repeatTimesOrDuration);

        static uint16_t getRemainTimesOrDuration(const WaveFile::Ptr& wavPtr, PlayType type, uint16_t repeatTimes, uint16_t deadTimeMs);

    private:
        std::unique_ptr<struct WavPlayerPrivate> m_p;
    };
}

#endif //AUTOSERVICES_WAV_PLAYER_HPP
