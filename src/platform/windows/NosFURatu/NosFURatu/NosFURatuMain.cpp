#include "pch.h"
#include "NosFURatuMain.h"
#include "DirectXHelper.h"
#include "GameScreenLevelEditor.h"
#include "GameScreenWorldMap.h"
#include "Game.h"
#include "SaveData.h"

#include <sstream>

using namespace NosFURatu;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System::Profile;
using namespace Windows::System::Threading;
using namespace Windows::UI::Notifications;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
NosFURatuMain::NosFURatuMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources), m_mediaPlayer(nullptr), m_iRequestedAction(0)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	AnalyticsVersionInfo^ api = AnalyticsInfo::VersionInfo;
	bool isMobile = api->DeviceFamily->Equals("Windows.Mobile");
	m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(m_deviceResources, isMobile, false));

	// Load Media Player
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(m_deviceResources->GetD3DDevice(), DXGI_FORMAT_B8G8R8A8_UNORM);

	// Load Sound Effects
	m_sounds.push_back("collect_carrot.wav");
	m_sounds.push_back("collect_golden_carrot.wav");
	m_sounds.push_back("death.wav");
	m_sounds.push_back("footstep_left_grass.wav");
	m_sounds.push_back("footstep_right_grass.wav");
	m_sounds.push_back("footstep_left_cave.wav");
	m_sounds.push_back("footstep_right_cave.wav");
	m_sounds.push_back("jump_spring.wav");
	m_sounds.push_back("landing_grass.wav");
	m_sounds.push_back("landing_cave.wav");
	m_sounds.push_back("snake_death.wav");
	m_sounds.push_back("trigger_transform.wav");
	m_sounds.push_back("cancel_transform.wav");
	m_sounds.push_back("complete_transform.wav");
	m_sounds.push_back("jump_spring_heavy.wav");
	m_sounds.push_back("jon_rabbit_jump.wav");
	m_sounds.push_back("jon_vampire_jump.wav");
	m_sounds.push_back("jon_rabbit_double_jump.wav");
	m_sounds.push_back("jon_rabbit_double_jump.wav");
	m_sounds.push_back("vampire_glide_loop.wav");
	m_sounds.push_back("mushroom_bounce.wav");
	m_sounds.push_back("jon_burrow_rocksfall.wav");
	m_sounds.push_back("sparrow_fly_loop.wav");
	m_sounds.push_back("sparrow_die.wav");
	m_sounds.push_back("toad_die.wav");
	m_sounds.push_back("toad_eat.wav");
	m_sounds.push_back("saw_grind_loop.wav");
	m_sounds.push_back("fox_bounced_on.wav");
	m_sounds.push_back("fox_strike.wav");
	m_sounds.push_back("fox_death.wav");
	m_sounds.push_back("world_1_bgm_intro.wav");
	m_sounds.push_back("mid_boss_bgm_intro.wav");
	m_sounds.push_back("mid_boss_owl_swoop.wav");
	m_sounds.push_back("mid_boss_owl_tree_smash.wav");
    m_sounds.push_back("mid_boss_owl_death.wav");
    m_sounds.push_back("screen_transition.wav");
    m_sounds.push_back("screen_transition_2.wav");
    m_sounds.push_back("level_complete.wav");
    m_sounds.push_back("title_lightning_1.wav");
    m_sounds.push_back("title_lightning_2.wav");
    m_sounds.push_back("ability_unlock.wav");
    m_sounds.push_back("boss_level_clear.wav");
    m_sounds.push_back("level_clear.wav");
    m_sounds.push_back("level_selected.wav");
    m_sounds.push_back("rabbit_drill.wav");
    m_sounds.push_back("snake_jump.wav");
    m_sounds.push_back("vampire_dash.wav");
    m_sounds.push_back("boss_level_unlock.wav");
    m_sounds.push_back("rabbit_stomp.wav");
}

NosFURatuMain::~NosFURatuMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void NosFURatuMain::CreateWindowSizeDependentResources()
{
	m_gameScreen->CreateWindowSizeDependentResources();
}

void NosFURatuMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_gameScreen->onResume();

	GameSound::getSoundPlayerInstance()->Resume();
}

void NosFURatuMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
	m_gameScreen->onPause();

	GameSound::getSoundPlayerInstance()->Suspend();

	if (m_mediaPlayer)
	{
		m_mediaPlayer->Pause();
	}
}

void NosFURatuMain::onTouchDown(float screenX, float screenY)
{
	m_gameScreen->onTouch(Touch_Type::DOWN, screenX, screenY);
}

void NosFURatuMain::onTouchDragged(float screenX, float screenY)
{
	m_gameScreen->onTouch(Touch_Type::DRAGGED, screenX, screenY);
}

void NosFURatuMain::onTouchUp(float screenX, float screenY)
{
	m_gameScreen->onTouch(Touch_Type::UP, screenX, screenY);
}

bool NosFURatuMain::handleOnBackPressed()
{
	return m_gameScreen->handleOnBackPressed();
}

// Notifies renderers that device resources need to be released.
void NosFURatuMain::OnDeviceLost()
{
	m_gameScreen->ReleaseDeviceDependentResources();

	m_mediaPlayer->Shutdown();
	m_mediaPlayer = nullptr;
}

// Notifies renderers that device resources may now be recreated.
void NosFURatuMain::OnDeviceRestored()
{
	m_gameScreen->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// Load Media Player
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(m_deviceResources->GetD3DDevice(), DXGI_FORMAT_B8G8R8A8_UNORM); 
}

void NosFURatuMain::Update()
{
	m_timer.Tick([&]()
	{
		int requestedAction = m_gameScreen->getRequestedAction();
		if (requestedAction >= 1000)
		{
			requestedAction /= 1000;
		}

		switch (requestedAction)
		{
		case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
			saveLevel(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
		case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
			loadLevel(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
		case REQUESTED_ACTION_LEVEL_COMPLETED:
			markLevelAsCompleted(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
        case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
            submitScoreOnline(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_UNLOCK_LEVEL:
            unlockLevel(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
            break;
        case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
            setCutsceneViewedFlag(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
            break;
		case REQUESTED_ACTION_GET_SAVE_DATA:
			sendSaveData();
			m_gameScreen->clearRequestedAction();
			break;
		case REQUESTED_ACTION_SHOW_MESSAGE:
			showMessage(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
		case REQUESTED_ACTION_UPDATE:
		default:
			break;
		}

		m_gameScreen->update(m_timer.GetElapsedSeconds());
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool NosFURatuMain::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Clear the back buffer
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);

	m_gameScreen->render();

	handleSound();
	handleMusic();

	return true;
}

void NosFURatuMain::handleSound()
{
	short soundId;
	while ((soundId = Assets::getInstance()->getFirstSoundId()) > 0)
	{
		Assets::getInstance()->eraseFirstSoundId();

		switch (soundId)
		{
		case SOUND_JON_VAMPIRE_GLIDE:
		case SOUND_SPARROW_FLY:
		case SOUND_SAW_GRIND:
			playSound(soundId, true);
			break;
		case STOP_SOUND_JON_VAMPIRE_GLIDE:
		case STOP_SOUND_SPARROW_FLY:
		case STOP_SOUND_SAW_GRIND:
			stopSound(soundId - 1000);
			break;
		default:
			playSound(soundId);
			break;
		}
	}
}

void NosFURatuMain::handleMusic()
{
	short rawMusicId = Assets::getInstance()->getMusicId();
	Assets::getInstance()->setMusicId(0);
	short musicId = rawMusicId;
	if (musicId >= 1000)
	{
		musicId /= 1000;
		rawMusicId -= musicId * 1000;
	}

	switch (musicId)
	{
	case MUSIC_STOP:
		if (m_mediaPlayer)
		{
			m_mediaPlayer->Pause();
		}
		break;
	case MUSIC_RESUME:
		m_mediaPlayer->Play();
		break;
	case MUSIC_SET_VOLUME:
		if (m_mediaPlayer)
		{
			float volume = rawMusicId / 100.0f / 2.0f; // On Win 10, volume starts off at 0.5
			if (volume < 0)
			{
				volume = 0;
			}

			m_mediaPlayer->SetVolume(volume);
		}
		break;
	case MUSIC_PLAY_TITLE_LOOP:
		m_threads.push_back(std::thread([](NosFURatuMain* nm)
		{
			nm->m_mediaPlayer->SetSource("title_bgm.wav");
			nm->m_mediaPlayer->Play();
		}, this));
		break;
    case MUSIC_PLAY_LEVEL_SELECT_LOOP:
        m_threads.push_back(std::thread([](NosFURatuMain* nm)
        {
            nm->m_mediaPlayer->SetSource("level_select_bgm.wav");
            nm->m_mediaPlayer->Play();
        }, this));
            break;
	case MUSIC_PLAY_WORLD_1_LOOP:
		m_threads.push_back(std::thread([](NosFURatuMain* nm)
		{
			nm->m_mediaPlayer->SetSource("world_1_bgm.wav");
			nm->m_mediaPlayer->Play();
		}, this));
		break; 
	case MUSIC_PLAY_MID_BOSS_LOOP:
		m_threads.push_back(std::thread([](NosFURatuMain* nm)
		{
			nm->m_mediaPlayer->SetSource("mid_boss_bgm.wav");
			nm->m_mediaPlayer->Play();
		}, this));
		break;
    case MUSIC_PLAY_END_BOSS_LOOP:
        m_threads.push_back(std::thread([](NosFURatuMain* nm)
        {
            nm->m_mediaPlayer->SetSource("final_boss_bgm.wav");
            nm->m_mediaPlayer->Play();
        }, this));
        break;
    case MUSIC_PLAY_OPENING_CUTSCENE:
        m_threads.push_back(std::thread([](NosFURatuMain* nm)
        {
            nm->m_mediaPlayer->SetSource("opening_cutscene_bgm.wav");
            nm->m_mediaPlayer->Play(false);
        }, this));
        break;
	default:
		break;
	}
}

void NosFURatuMain::playSound(int soundId, bool isLoop)
{
	m_sounds.at(soundId - 1).play(isLoop);
}

void NosFURatuMain::stopSound(int soundId)
{
	m_sounds.at(soundId - 1).stop();
}

void NosFURatuMain::saveLevel(int requestedAction)
{
	m_iRequestedAction = requestedAction;

	create_task(KnownFolders::PicturesLibrary->GetFoldersAsync()).then([this](IVectorView<StorageFolder^>^ folders)
	{
		bool hasNosfuratuLevelsFolder = false;
		std::for_each(begin(folders), end(folders), [&](StorageFolder^ value)
		{
			if (value->Name->Equals(L"nosfuratu-levels"))
			{
				hasNosfuratuLevelsFolder = true;
			}
		});

		if (hasNosfuratuLevelsFolder)
		{
			create_task(KnownFolders::PicturesLibrary->GetFolderAsync(L"nosfuratu-levels")).then([this](StorageFolder^ folder)
			{
				Platform::String^ filename = getLevelName(m_iRequestedAction);
				create_task(folder->CreateFileAsync(filename, CreationCollisionOption::ReplaceExisting)).then([this](StorageFile^ file)
				{
					if (file != nullptr)
					{
						const char *level_json = GameScreenLevelEditor::getInstance()->save();
						std::string s(level_json);
						std::wstring ws;
						ws.assign(s.begin(), s.end());
						Platform::String^ levelJson = ref new Platform::String(ws.c_str());
						if (levelJson != nullptr && !levelJson->IsEmpty())
						{
							create_task(FileIO::WriteTextAsync(file, levelJson)).then([this, file, levelJson](task<void> task)
							{
								try
								{
									task.get();

									displayToast(L"Level saved successfully");
								}
								catch (Platform::COMException^ ex)
								{
									displayToast(L"Error occurred while saving level... Please try again!");
								}
							});
						}
						else
						{
							displayToast(L"Error occurred while saving level... Please try again!");
						}
					}
				});
			});
		}
		else
		{
			displayToast(L"Move the nosfuratu-levels repo to your Pictures folder and try again!");
		}
	});
}

void NosFURatuMain::loadLevel(int requestedAction)
{
	m_iRequestedAction = requestedAction;

	create_task(KnownFolders::PicturesLibrary->GetFoldersAsync()).then([this](IVectorView<StorageFolder^>^ folders)
	{
		bool hasNosfuratuLevelsFolder = false;
		std::for_each(begin(folders), end(folders), [&](StorageFolder^ value)
		{
			if (value->Name->Equals(L"nosfuratu-levels"))
			{
				hasNosfuratuLevelsFolder = true;
			}
		});

		if (hasNosfuratuLevelsFolder)
		{
			create_task(KnownFolders::PicturesLibrary->GetFolderAsync(L"nosfuratu-levels")).then([this](StorageFolder^ folder)
			{
				Platform::String^ filename = getLevelName(m_iRequestedAction);
				create_task(folder->CreateFileAsync(filename, CreationCollisionOption::OpenIfExists)).then([this](StorageFile^ file)
				{
					if (file != nullptr)
					{
						create_task(FileIO::ReadTextAsync(file)).then([this, file](task<Platform::String^> task)
						{
							try
							{
								Platform::String^ fileContent = task.get();
								if (fileContent != nullptr)
								{
									std::wstring fooW(fileContent->Begin());
									std::string fooA(fooW.begin(), fooW.end());
									const char *levelContent = fooA.c_str();
									GameScreenLevelEditor::getInstance()->load(levelContent, m_gameScreen.get());

									displayToast(L"Level loaded successfully");
								}
								else
								{
									displayToast(L"Error occurred while loading level...");
								}
							}
							catch (Platform::COMException^ ex)
							{
								displayToast(L"Error occurred while loading level...");
							}
						});
					}
				});
			});
		}
		else
		{
			displayToast(L"Move the nosfuratu-levels repo to your Pictures folder and try again!");
		}
	});
}

void NosFURatuMain::unlockLevel(int requestedAction)
{
    int world = calcWorld(requestedAction);
    int level = calcLevel(requestedAction);
    int score = m_gameScreen->getScore();
    int levelStatsFlag = m_gameScreen->getLevelStatsFlagForUnlockedLevel();
    int numGoldenCarrots = m_gameScreen->getNumGoldenCarrotsAfterUnlockingLevel();
    
    SaveData::setLevelStatsFlag(world, level, levelStatsFlag);
    
    SaveData::setNumGoldenCarrots(numGoldenCarrots);
}

void NosFURatuMain::markLevelAsCompleted(int requestedAction)
{
	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);
    int score = m_gameScreen->getScore();
    int levelStatsFlag = m_gameScreen->getLevelStatsFlag();
    int numGoldenCarrots = m_gameScreen->getNumGoldenCarrots();
    int jonUnlockedAbilitiesFlag = m_gameScreen->getJonAbilityFlag();

	SaveData::setLevelComplete(world, level, score, levelStatsFlag, jonUnlockedAbilitiesFlag);
    
    SaveData::setNumGoldenCarrots(numGoldenCarrots);
}

void NosFURatuMain::submitScoreOnline(int requestedAction)
{
    int world = calcWorld(requestedAction);
    int level = calcLevel(requestedAction);
    int onlineScore = m_gameScreen->getOnlineScore();
    
    // TODO, submit score using Xbox Live or OpenXLive, on success, save the score that was pushed online
    
    SaveData::setScorePushedOnline(world, level, onlineScore);
}

void NosFURatuMain::setCutsceneViewedFlag(int requestedAction)
{
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    int cutsceneViewedFlag = requestedAction;
    
    SaveData::setViewedCutscenesFlag(cutsceneViewedFlag);
}

void NosFURatuMain::sendSaveData()
{
    int numGoldenCarrots = SaveData::getNumGoldenCarrots();
    int jonUnlockedAbilitiesFlag = SaveData::getJonUnlockedAbilitiesFlag();
    int viewedCutscenesFlag = SaveData::getViewedCutscenesFlag();
    
    std::stringstream ss;
    ss << "{";
    ss << "\"num_golden_carrots\": " << numGoldenCarrots << ", ";
    ss << "\"jon_unlocked_abilities_flag\": " << jonUnlockedAbilitiesFlag << ", ";
    ss << "\"viewed_cutscenes_flag\": " << viewedCutscenesFlag << ", ";
    
    for (int i = 1; i <= 5; i++)
    {
        ss << "\"world_" << i << "\":[";
        
        for (int j = 1; j <= 21; j++)
        {
            int statsFlag = SaveData::getLevelStatsFlag(i, j);
            int score = SaveData::getLevelScore(i, j);
            int scoreOnline = SaveData::getScorePushedOnline(i, j);
            
            ss << "{";
            ss << "\"stats_flag\": " << statsFlag << ", ";
            ss << "\"score\": " << score << ", ";
            ss << "\"score_online\": " << scoreOnline << " ";
            
            ss << "}";
            if (j < 21)
            {
                ss << ",";
            }
            
            ss << " ";
        }
        ss << "]";
        if (i < 5)
        {
            ss << ",";
        }
    }
    ss << "}";
    
    std::string userSaveData = ss.str();
    
    WorldMap::getInstance()->loadUserSaveData(userSaveData.c_str());
}

void NosFURatuMain::showMessage(int requestedAction)
{
    while (requestedAction >= 1000)
	{
		requestedAction -= 1000;
	}

	int messageKey = requestedAction;

	const char* toast = nullptr;

	switch (messageKey)
	{
	case MESSAGE_NO_END_SIGN_KEY:
		toast = MESSAGE_NO_END_SIGN_VAL;
		break;
	case MESSAGE_NO_JON_KEY:
		toast = MESSAGE_NO_JON_VAL;
		break;
	case MESSAGE_INVALID_JON_KEY:
		toast = MESSAGE_INVALID_JON_VAL;
		break;
	case MESSAGE_NO_COUNT_HISS_KEY:
		toast = MESSAGE_NO_COUNT_HISS_VAL;
		break;
	case MESSAGE_INVALID_COUNT_HISS_KEY:
		toast = MESSAGE_INVALID_COUNT_HISS_VAL;
		break;
	case MESSAGE_OFFSET_NEEDS_MARKERS_KEY:
		toast = MESSAGE_OFFSET_NEEDS_MARKERS_VAL;
		break;
	case MESSAGE_FEATURE_COMING_SOON_KEY:
		toast = MESSAGE_FEATURE_COMING_SOON_VAL;
		break;
	default:
		break;
	}

	if (toast)
	{
		std::string s_str = std::string(toast);
		std::wstring wid_str = std::wstring(s_str.begin(), s_str.end());
		const wchar_t* w_char = wid_str.c_str();
		Platform::String^ p_string = ref new Platform::String(w_char);

		displayToast(p_string);
	}
}

Platform::String^ NosFURatuMain::getLevelName(int requestedAction)
{
	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);

	if (world > 0 && level > 0)
	{
		return L"nosfuratu_c" + world + L"_l" + level + L".json";
	}
	else
	{
		return L"nosfuratu.json";
	}
}

int NosFURatuMain::calcWorld(int requestedAction)
{
    int world = 0;
    
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
        world++;
    }
    
    return world;
}

int NosFURatuMain::calcLevel(int requestedAction)
{
    int level = 0;
    
    while (requestedAction >= 1000)
    {
        requestedAction -= 1000;
    }
    
    while (requestedAction >= 100)
    {
        requestedAction -= 100;
    }
    
    level = requestedAction;
    
    return level;
}

void NosFURatuMain::displayToast(Platform::String^ message)
{
	Platform::String^ toast = "<toast>"
		+ "<visual>"
		+ "<binding template = \"ToastGeneric\" >"
		+ "<text>"
		+ message
		+ "</text>"
		+ "</binding>"
		+ "</visual>"
		+ "</toast>";

	Windows::Data::Xml::Dom::XmlDocument^ toastDOM = ref new Windows::Data::Xml::Dom::XmlDocument();
	toastDOM->LoadXml(toast);

	ToastNotification^ toastNotification = ref new ToastNotification(toastDOM);

	Windows::UI::Notifications::ToastNotifier^ toastNotifier = Windows::UI::Notifications::ToastNotificationManager::CreateToastNotifier();
	toastNotifier->Show(toastNotification);
}
