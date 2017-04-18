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
NosFURatuMain::NosFURatuMain(DirectXPage^ directXPage, const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_directXPage(directXPage), m_deviceResources(deviceResources), m_mediaPlayer(nullptr), m_iRequestedAction(0)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	bool isMobile = false;
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	isMobile = true;
#endif

	m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(m_deviceResources, isMobile, isMobile));

	initSoundEngine();
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
		case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
			m_gameScreen->clearRequestedAction();
			m_directXPage->DisplayInterstitialAdIfAvailable();
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
	while ((soundId = m_gameScreen->getCurrentSoundId()) > 0)
	{
		switch (soundId)
		{
		case SOUND_JON_VAMPIRE_GLIDE:
		case SOUND_SPARROW_FLY:
		case SOUND_SAW_GRIND:
        case SOUND_SPIKED_BALL_ROLLING:
			playSound(soundId, true);
			break;
		case STOP_SOUND_JON_VAMPIRE_GLIDE:
		case STOP_SOUND_SPARROW_FLY:
		case STOP_SOUND_SAW_GRIND:
        case STOP_SOUND_SPIKED_BALL_ROLLING:
			stopSound(soundId - 1000);
			break;
        case STOP_ALL_SOUNDS:
            stopAllSounds();
            break;
        case STOP_ALL_LOOPING_SOUNDS:
            stopAllLoopingSounds();
            break;
		default:
			playSound(soundId);
			break;
		}
	}
}

void NosFURatuMain::handleMusic()
{
	short rawMusicId;
	while ((rawMusicId = m_gameScreen->getCurrentMusicId()) > 0)
	{
		short musicId = rawMusicId;
		if (musicId >= 1000)
		{
			musicId /= 1000;
			rawMusicId -= musicId * 1000;
		}

		if (m_mediaPlayer)
		{
			switch (musicId)
			{
			case MUSIC_STOP:
				m_mediaPlayer->Pause();
				break;
			case MUSIC_RESUME:
				m_mediaPlayer->Play();
				break;
			case MUSIC_PLAY:
				m_mediaPlayer->Play(false);
				break;
			case MUSIC_PLAY_LOOP:
				m_mediaPlayer->Play();
				break;
			case MUSIC_SET_VOLUME:
			{
				float volume = rawMusicId / 100.0f / 2.0f; // On Win 10, volume starts off at 0.5

				m_mediaPlayer->SetVolume(volume);
			}
			break;
			case MUSIC_LOAD_TITLE_LOOP:
				m_mediaPlayer->SetSource("title_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			case MUSIC_LOAD_LEVEL_SELECT_LOOP:
				m_mediaPlayer->SetSource("level_select_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			case MUSIC_LOAD_WORLD_1_LOOP:
				m_mediaPlayer->SetSource("world_1_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			case MUSIC_LOAD_MID_BOSS_LOOP:
				m_mediaPlayer->SetSource("mid_boss_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			case MUSIC_LOAD_END_BOSS_LOOP:
				m_mediaPlayer->SetSource("final_boss_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			case MUSIC_LOAD_OPENING_CUTSCENE:
				m_mediaPlayer->SetSource("opening_cutscene_bgm.wav");
				m_mediaPlayer->SetCurrentTime(0);
				break;
			default:
				break;
			}
		}
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

void NosFURatuMain::stopAllSounds(bool stopOnlyLoopingSounds)
{
    for (std::vector<GameSound>::iterator i = m_sounds.begin(); i != m_sounds.end(); i++)
    {
        if (!stopOnlyLoopingSounds
            || (stopOnlyLoopingSounds
                && (*i).isLooping()))
        {
            (*i).stop();
        }
    }
}

void NosFURatuMain::stopAllLoopingSounds()
{
    stopAllSounds(true);
}

void NosFURatuMain::initSoundEngine()
{
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
	m_sounds.push_back("final_boss_bgm_intro.wav");
	m_sounds.push_back("button_click.wav");
	m_sounds.push_back("level_confirmed.wav");
	m_sounds.push_back("bat_poof.wav");
	m_sounds.push_back("chain_snap.wav");
	m_sounds.push_back("end_boss_snake_mouth_open.wav");
	m_sounds.push_back("end_boss_snake_charge_cue.wav");
	m_sounds.push_back("end_boss_snake_charge.wav");
	m_sounds.push_back("end_boss_snake_damaged.wav");
	m_sounds.push_back("end_boss_snake_death.wav");
	m_sounds.push_back("spiked_ball_rolling_loop.wav");
	m_sounds.push_back("absorb_dash_ability.wav");
	m_sounds.push_back("footstep_left_wood.wav");
	m_sounds.push_back("footstep_right_wood.wav");
	m_sounds.push_back("landing_wood.wav");
}