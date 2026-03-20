#include "player/wav_player.hpp"
#include "player_controller.hpp"
#include "player/player_factory.hpp"
#include "resourcemanager/resource_manager.hpp"
#include "soundclient/client_vsomeip.hpp"

#include <thread>
#include <condition_variable>
#include <atomic>
#include <list>
#include <set>

namespace AutoSoundService
{
    struct PlayParamsGreater
    {
        bool operator()(const std::shared_ptr<PlayParams>& t1,const std::shared_ptr<PlayParams>& t2) const
        {
            return t1->soundType > t2->soundType;    //>=: ascending order; >: descending order
        }
    };


    struct PlayerControllerPrivate
    {
        PlayerFactory               factory;
        PlayerBase::Ptr             player;
        SoundState                  state;

        PlayParams::Ptr                                 currentTask{nullptr};
        std::set<PlayParams::Ptr, PlayParamsGreater>    playTaskLists;
        std::mutex                                      playMutex;
        std::thread                                     worker;
        std::atomic<bool>                               stop{true};
        std::condition_variable                         cv;
        std::unique_ptr<VSomeipClient>                  clientAndroid;
    };

    PlayerController::PlayerController()
        : m_p(new PlayerControllerPrivate)
    {
        m_p->clientAndroid.reset(new VSomeipClient(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID));
        m_p->factory.registerPlayer(PlayerType::WAV, std::bind(&PlayerController::onStateChange, this, std::placeholders::_1));
    }

    PlayerController::~PlayerController()
    {
        if (!m_p->stop)
        {
            this->stop();
        }
    }

    void PlayerController::start()
    {
        if (m_p->stop)
        {
            m_p->stop = false;
            m_p->worker = std::thread([this] { this->playWorker(); });
            auto threadId = m_p->worker.get_id();
            logInfo("[PlayerController::start]: Play worker thread start success, id: %u", *(unsigned int*)&threadId);
        }
        else
        {
            logWarning("[PlayerController::start]: PlayerController has already started!");
        }

        if(m_p->clientAndroid->init())
        {
            m_p->clientAndroid->start();
        }
        else
        {
            logError("Init client for Android failed!");
        }
    }

    void PlayerController::stop()
    {
        m_p->stop = true;
        {
            std::unique_lock<std::mutex> lck(m_p->playMutex);
            m_p->playTaskLists.clear(); // clear stack
            if (m_p->currentTask)
            {
                auto player = m_p->factory.getPlayer(ResourceManager::GetPlayerType(m_p->currentTask->soundType));
                if (player && player->isBusy())
                {
                    player->stop(StopType::IMMEDIATELY);
                }
            }
            m_p->currentTask.reset();
        }
        m_p->cv.notify_all();

        if (m_p->worker.joinable())
        {
            m_p->worker.join();
        }

        m_p->clientAndroid->stop();
        logInfo("[PlayerController::stop]: Play worker thread stop success");
    }

    void PlayerController::addTask(const PlayParams &playParams)
    {
        // parse params
        PlayParams params;
        AudioFiles audioFiles = ResourceManager::GetAudioFilePath(playParams.soundType);
        params.soundType                = playParams.soundType;
        params.playType                 = playParams.playType;
        params.playerType               = ResourceManager::GetPlayerType(playParams.soundType);
        params.repeatTimesOrDuration    = playParams.repeatTimesOrDuration;
        params.fileName                 = ResourceManager::SoundType2String(playParams.soundType);
        params.intervalMs               = playParams.intervalMs > 0 ? playParams.intervalMs : audioFiles.interval_ms;
        params.audioPaths               = audioFiles.pathContainer;

        // process params
        {
            std::unique_lock<std::mutex> lck(m_p->playMutex);

            if (m_p->currentTask)
            {
                // check if it needs drop request
                if (m_p->currentTask->soundType >= params.soundType)
                {
                    if (PlayType::REPEAT == params.playType)
                    {// add to task list
                        m_p->playTaskLists.insert(std::make_shared<PlayParams>(params));
                    }
                    return;// drop request
                }
                // get current player
                PlayerBase::Ptr player;
                if (m_p->currentTask && m_p->currentTask->soundType == m_p->currentTask->soundType)
                {
                    player = m_p->factory.getPlayer(ResourceManager::GetPlayerType(m_p->currentTask->soundType));
                }
                // switch to new jobs
                logInfo("[PlayerController::addTask]: switch soundType: %d -> %d", (int)m_p->currentTask->playType, (int)params.soundType);
                if (m_p->currentTask->playType == PlayType::REPEAT)
                {
                    logInfo("[PlayerController::addTask]: store current task to stack");
                    m_p->playTaskLists.insert(m_p->currentTask);
                }
                m_p->currentTask = std::make_shared<PlayParams>(params);
                // stop current player
                if (player && player->isBusy())
                {
                    m_p->player->stop(StopType::IMMEDIATELY);
                }
            }
            else
            {
                m_p->currentTask = std::make_shared<PlayParams>(params);
            }
        }
        m_p->cv.notify_all();
    }

    void PlayerController::removeTask(const StopParams &stopParams)
    {
        {
            std::unique_lock<std::mutex> lck(m_p->playMutex);
            PlayParams params;
            params.soundType = stopParams.soundType;
            m_p->playTaskLists.erase(std::make_shared<PlayParams>(params));
            //("[PlayerController::removeTask]: remove play task: %u, current task stack size: %lu",
                     (uint)stopParams.soundType, m_p->playTaskLists.size());

            if (m_p->currentTask && m_p->currentTask->soundType == stopParams.soundType)
            {
                auto player = m_p->factory.getPlayer(ResourceManager::GetPlayerType(m_p->currentTask->soundType));
                if (player && player->isBusy())
                {
                    player->stop(stopParams.stopType);
                }
                m_p->currentTask.reset();
            }
        }
        m_p->cv.notify_all();
    }

    void PlayerController::reset()
    {
        this->stop();
        // sleep 10 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        this->start();
        logInfo("[PlayerController::reset]: PlayerController reset success!");
    }

    void PlayerController::playWorker()
    {
        while (!m_p->stop)
        {
            PlayParams params;
            {
                std::unique_lock<std::mutex> lck(m_p->playMutex);
                m_p->cv.wait(lck, [this]() ->bool {
                    return m_p->currentTask != nullptr || !m_p->playTaskLists.empty() || m_p->stop;
                });
                if (m_p->stop)
                {
                    break;
                }
                // get play task from stack
                if (nullptr == m_p->currentTask)
                {
                    m_p->currentTask = *m_p->playTaskLists.begin();
                    m_p->playTaskLists.erase(m_p->playTaskLists.begin()); // remove
                }
                params = *m_p->currentTask;
            }
            m_p->player = m_p->factory.getPlayer(params.playerType);
            if(m_p->player)
                m_p->player->play(params);
            else
                logWarning("PlayerController::playWorker can't find player!!!");

            {
                std::unique_lock<std::mutex> lck(m_p->playMutex);
                if (m_p->currentTask && m_p->currentTask->soundType == params.soundType)
                {
                    m_p->currentTask.reset();
                    m_p->currentTask = nullptr;
                }
            }


        }
        //logDebug("[PlayerController] thread PlayerController::playWorker end");
    } 

    void PlayerController::onStateChange(const SoundState &state)
    {
        //logDebug("[PlayerController::onStateChange] name = %s, PlayPriority: %d, PlayerType: %s, "
                 "PlayType: %d, remainTimesOrDuration: %d, PlayState: %d",
                 state.name.c_str(), static_cast<uint8_t>(state.priority), ResourceManager::GetPlayerName(state.playerType).c_str(),
                 state.playerType, state.remainTimesOrDuration, static_cast<uint8_t>(state.state));
        std::vector<uint8_t> data;
        data.push_back(1);
        data.push_back(3);
        if (PlayState::PLAYING == state.state)
        {
            data.push_back(1);
        }
        else if (PlayState::IDLE == state.state)
        {
            data.push_back(2);
        }
        if (!m_p->clientAndroid->send(data))
        {
            logError("Send sound state to Android failed!");
            printf("Send sound state to Android failed!\n");
        }

        // update sound status
        m_p->state = state;
    }
}