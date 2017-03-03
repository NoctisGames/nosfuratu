//
// Direct3DMain.cpp
//

#include "pch.h"

#include "Direct3DMain.h"

#include "Direct3DManager.h"
#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "GamePadInputManager.h"
#include "SoundManager.h"
#include "GameConstants.h"
#include "macros.h"
#include "Game.h"
#include "MainScreenWorldMap.h"
#include "MainScreenLevelEditor.h"
#include "SaveData.h"
#include "MainAssets.h"

#include <sstream>

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Direct3DMain::Direct3DMain() : m_screen(nullptr), m_fDPI(0), m_iRequestedAction(0), m_isPointerPressed(false), m_retryAudio(false), m_isDeviceLost(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);

	Direct3DManager::setDeviceResources(m_deviceResources.get());

	// Hide Constructor for Singleton
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	m_isWindowsMobile = false;
#else
	Windows::System::Profile::AnalyticsVersionInfo^ api = Windows::System::Profile::AnalyticsInfo::VersionInfo;
	m_isWindowsMobile = api->DeviceFamily->Equals("Windows.Mobile");
#endif

	MAIN_ASSETS->setUsingDesktopTextureSet(!m_isWindowsMobile);

	m_screen = new MainScreen();
}

Direct3DMain::~Direct3DMain()
{
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}

	m_musicLoop.reset();

	delete m_screen;

	m_deviceResources.reset();
}

// Initialize the Direct3D resources required to run.
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
void Direct3DMain::Initialize(HWND window, int width, int height)
{
	m_deviceResources->SetWindow(window, width, height);
#else
void Direct3DMain::Initialize(IUnknown* window, int width, int height, float dpi, DXGI_MODE_ROTATION rotation)
{
	m_fDPI = dpi;
	m_deviceResources->SetWindow(window, width, height, rotation);
#endif

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	m_keyboard = std::make_unique<Keyboard>();

	m_mouse = std::make_unique<Mouse>();
	m_mouse->SetMode(Mouse::MODE_ABSOLUTE); // Use MODE_RELATIVE for displaying your own mouse pointer (like Diablo or Age of Empires)

	m_gamePad = std::make_unique<GamePad>();

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	m_mouse->SetWindow(window);
#else
	m_mouse->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
	m_keyboard->SetWindow(reinterpret_cast<ABI::Windows::UI::Core::ICoreWindow*>(window));
#endif

	initSoundEngine();
}

void Direct3DMain::OnNewAudioDevice()
{
	m_retryAudio = true;
}

#pragma region Frame Update
// Executes the basic game loop.
void Direct3DMain::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Direct3DMain::Update(DX::StepTimer const& timer)
{
	auto kb = m_keyboard->GetState();
	m_keys.Update(kb);
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Right))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Right))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Up))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Up))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Left))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Left))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Down))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Down))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::W))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::W))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::A))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::A))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::S))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::S))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::D))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::D))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Enter))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Enter))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Space))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Space))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE, true);
	}
	if (m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Back)
		|| m_keys.IsKeyPressed(DirectX::Keyboard::Keys::Delete))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK);
	}
	else if (m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Back)
		|| m_keys.IsKeyReleased(DirectX::Keyboard::Keys::Delete))
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
	}

	auto mouse = m_mouse->GetState();
	if (mouse.positionMode == Mouse::MODE_ABSOLUTE)
	{
		if (m_isPointerPressed && mouse.leftButton)
		{
			SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, float(mouse.x), float(mouse.y));
		}
		else if (mouse.leftButton && !m_isPointerPressed)
		{
			SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, float(mouse.x), float(mouse.y));

			m_isPointerPressed = true;
		}
		else if (m_isPointerPressed && !mouse.leftButton)
		{
			SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, float(mouse.x), float(mouse.y));

			m_isPointerPressed = false;
		}
	}

	auto gamePadState = m_gamePad->GetState(0);
	if (gamePadState.IsConnected())
	{
        m_buttons.Update(gamePadState);

		if (m_buttons.dpadRight == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_RIGHT, 0, 1);
		}
		else if (m_buttons.dpadRight == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_RIGHT, 0);
		}
		if (m_buttons.dpadUp == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_UP, 0, 1);
		}
		else if (m_buttons.dpadUp == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_UP, 0);
		}
		if (m_buttons.dpadLeft == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_LEFT, 0, 1);
		}
		else if (m_buttons.dpadLeft == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_LEFT, 0);
		}
		if (m_buttons.dpadDown == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_DOWN, 0, 1);
		}
		else if (m_buttons.dpadDown == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_D_PAD_DOWN, 0);
		}
        
		if (m_buttons.a == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_A_BUTTON, 0, 1);
		}
		else if (m_buttons.a == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_A_BUTTON, 0);
		}
		if (m_buttons.b == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_B_BUTTON, 0, 1);
		}
		else if (m_buttons.b == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_B_BUTTON, 0);
		}
		if (m_buttons.x == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_X_BUTTON, 0, 1);
		}
		else if (m_buttons.x == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_X_BUTTON, 0);
		}
		if (m_buttons.y == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_Y_BUTTON, 0, 1);
		}
		else if (m_buttons.y == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_Y_BUTTON, 0);
		}

		if (m_buttons.start == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_START_BUTTON, 0, 1);
		}
		else if (m_buttons.start == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_START_BUTTON, 0);
		}
		if (m_buttons.back == GamePad::ButtonStateTracker::PRESSED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_BACK_BUTTON, 0, 1);
		}
		else if (m_buttons.back == GamePad::ButtonStateTracker::RELEASED)
		{
			GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_BACK_BUTTON, 0);
		}
        
		GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_TRIGGER, 0, gamePadState.triggers.left, gamePadState.triggers.right);
		GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_STICK_LEFT, 0, gamePadState.thumbSticks.leftX, gamePadState.thumbSticks.leftY);
		GAME_PAD_INPUT_MANAGER->onInput(GamePadEventType_STICK_RIGHT, 0, gamePadState.thumbSticks.rightX, gamePadState.thumbSticks.rightY);
	}

	if (m_retryAudio)
	{
		m_retryAudio = false;
		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here
		}
	}
	else if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			m_retryAudio = true;
		}
	}

	beginPixEvent(L"Update");

	int requestedAction = m_screen->getRequestedAction();
	if (requestedAction >= 1000)
	{
		requestedAction /= 1000;
	}

	switch (requestedAction)
	{
	case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
		saveLevel(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
		loadLevel(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_LEVEL_COMPLETED:
		markLevelAsCompleted(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
		submitScoreOnline(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_UNLOCK_LEVEL:
		unlockLevel(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
		setCutsceneViewedFlag(m_screen->getRequestedAction());
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_GET_SAVE_DATA:
		sendSaveData();
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
		displayInterstitialAdIfAvailable();
		m_screen->clearRequestedAction();
		break;
	case REQUESTED_ACTION_UPDATE:
	default:
		break;
	}

	float elapsedTime = float(timer.GetElapsedSeconds());

	m_screen->update(elapsedTime);

	endPixEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Direct3DMain::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

	beginPixEvent(L"Render", m_deviceResources.get());

	m_screen->render();

	endPixEvent(m_deviceResources.get());

	handleSound();
	handleMusic();

    // Show the new frame.
	beginPixEvent(L"Present");
	m_deviceResources->Present();
	endPixEvent();
}

// Helper method to clear the back buffers.
void Direct3DMain::Clear()
{
	beginPixEvent(L"Clear", m_deviceResources.get());

	// Clear the views.
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto depthStencil = m_deviceResources->GetDepthStencilView();

	context->ClearRenderTargetView(renderTarget, Colors::Black);
	context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &renderTarget, depthStencil);

	// Set the viewport.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	endPixEvent(m_deviceResources.get());
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Direct3DMain::OnActivated()
{
	// Game is becoming active window.

	m_keys.Reset();
	m_gamePad->Resume();
	m_buttons.Reset();

	m_audEngine->Resume();

	if (m_isWindowsMobile
		&& m_isDeviceLost)
	{
		OnDeviceRestored();
	}

	m_screen->onResume();
}

void Direct3DMain::OnDeactivated()
{
	// Game is becoming background window.

	m_keys.Reset();
	m_gamePad->Suspend();
	m_buttons.Reset();

	m_audEngine->Suspend();

	if (m_isWindowsMobile)
	{
		OnDeviceLost();

		m_isDeviceLost = true;
	}

	m_screen->onPause();
}

void Direct3DMain::OnSuspending()
{
	// Game is being power-suspended (or minimized).

	m_keys.Reset();
	m_gamePad->Suspend();
	m_buttons.Reset();

	m_audEngine->Suspend();

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	// Empty
#else
	auto context = m_deviceResources->GetD3DDeviceContext();
	context->ClearState();

	m_deviceResources->Trim();
#endif

	m_screen->onPause();
}

void Direct3DMain::OnResuming()
{
	// Game is being power-resumed (or returning from minimize).

	m_keys.Reset();
	m_gamePad->Resume();
	m_buttons.Reset();

	m_timer.ResetElapsedTime();
	
	m_audEngine->Resume();

	m_screen->onResume();
}

#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
void Direct3DMain::OnWindowSizeChanged(int width, int height)
{
	if (!m_deviceResources->WindowSizeChanged(width, height))
	{
		return;
	}

	CreateWindowSizeDependentResources();
}
#else
void Direct3DMain::OnWindowSizeChanged(int width, int height, float dpi, DXGI_MODE_ROTATION rotation)
{
	m_fDPI = dpi;

	if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
	{
		return;
	}

	CreateWindowSizeDependentResources();
}

void Direct3DMain::ValidateDevice()
{
	m_deviceResources->ValidateDevice();
}
#endif

// Properties
void Direct3DMain::GetDefaultSize(int& width, int& height) const
{
	width = 800;
    height = 600;
}

MainScreen* Direct3DMain::getMainScreen()
{
	return m_screen;
}

int Direct3DMain::getRequestedAction()
{
	return m_iRequestedAction;
}

void Direct3DMain::clearRequestedAction()
{
	m_iRequestedAction = 0;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Direct3DMain::CreateDeviceDependentResources()
{
	m_screen->createDeviceDependentResources();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Direct3DMain::CreateWindowSizeDependentResources()
{
	RECT outputSize = m_deviceResources->GetOutputSize();
	LONG width = outputSize.right - outputSize.left;
	LONG height = outputSize.bottom - outputSize.top;
	LONG touchWidth = width;
	LONG touchHeight = height;

	if (D3DManager->isWindowsMobile())
	{
		touchWidth = height;
		touchHeight = width;
	}

	m_screen->createWindowSizeDependentResources(width, height, touchWidth, touchHeight);
}

void Direct3DMain::beginPixEvent(PCWSTR pFormat, DX::DeviceResources* deviceResources)
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	UNUSED(deviceResources);

	m_deviceResources->PIXBeginEvent(pFormat);
#else
	if (deviceResources)
	{
		auto context = deviceResources->GetD3DDeviceContext();
		PIXBeginEvent(context, PIX_COLOR_DEFAULT, pFormat);
	}
	else
	{
		PIXBeginEvent(PIX_COLOR_DEFAULT, pFormat);
	}
#endif
}

void Direct3DMain::endPixEvent(DX::DeviceResources* deviceResources)
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	UNUSED(deviceResources);

	m_deviceResources->PIXEndEvent();
#else
	if (deviceResources)
	{
		auto context = deviceResources->GetD3DDeviceContext();
		PIXEndEvent(context);
	}
	else
	{
		PIXEndEvent();
	}
#endif
}

void Direct3DMain::OnDeviceLost()
{
	m_screen->releaseDeviceDependentResources();
}

void Direct3DMain::OnDeviceRestored()
{
	CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}
#pragma endregion

#pragma region Audio
void Direct3DMain::handleSound()
{
	short soundId;
	while ((soundId = SOUND_MANAGER->getCurrentSoundId()) > SOUND_NONE)
	{
		switch (soundId)
		{
		case SOUND_JON_VAMPIRE_GLIDE:
		case SOUND_SPARROW_FLY:
		case SOUND_SAW_GRIND:
		case SOUND_SPIKED_BALL_ROLLING:
			// TODO, make looping
			m_sounds.at(soundIndexForSoundId(soundId))->Play();
			break;
		case STOP_SOUND_JON_VAMPIRE_GLIDE:
		case STOP_SOUND_SPARROW_FLY:
		case STOP_SOUND_SAW_GRIND:
		case STOP_SOUND_SPIKED_BALL_ROLLING:
			// TODO, soundId - 1000
			break;
		case RESUME_ALL_SOUNDS:
			// TODO
			break;
		case PAUSE_ALL_SOUNDS:
			// TODO
			break;
		case STOP_ALL_SOUNDS:
			// TODO
			break;
		case STOP_ALL_LOOPING_SOUNDS:
			// TODO
			break;
		default:
			m_sounds.at(soundIndexForSoundId(soundId))->Play();
			break;
		}
	}
}

void Direct3DMain::handleMusic()
{
	short musicId;
    while ((musicId = SOUND_MANAGER->getCurrentMusicId()) > MUSIC_NONE)
    {
		if (musicId > MUSIC_SET_VOLUME)
		{
			if (m_musicLoop)
			{
				m_musicLoop->SetVolume((musicId % 1000) / 100.0f);
			}
		}
		else
		{
			switch (musicId)
			{
			case MUSIC_STOP:
				if (m_musicLoop)
				{
					m_musicLoop->Pause();
				}
				break;
			case MUSIC_RESUME:
				if (m_musicLoop)
				{
					m_musicLoop->Resume();
				}
				break;
			case MUSIC_PLAY:
				if (m_musicLoop)
				{
					m_musicLoop->SetVolume(1);
					m_musicLoop->Play();
				}
				break;
			case MUSIC_PLAY_LOOP:
				if (m_musicLoop)
				{
					m_musicLoop->SetVolume(1);
					m_musicLoop->Play(true);
				}
				break;
			case MUSIC_LOAD_TITLE_LOOP:
				loadMusic(L"title_bgm.wav");
				break;
			case MUSIC_LOAD_LEVEL_SELECT_LOOP:
				loadMusic(L"level_select_bgm.wav");
				break;
			case MUSIC_LOAD_WORLD_1_LOOP:
				loadMusic(L"world_1_bgm.wav");
				break;
			case MUSIC_LOAD_MID_BOSS_LOOP:
				loadMusic(L"mid_boss_bgm.wav");
				break;
			case MUSIC_LOAD_END_BOSS_LOOP:
				loadMusic(L"final_boss_bgm.wav");
				break;
			case MUSIC_LOAD_OPENING_CUTSCENE:
				loadMusic(L"opening_cutscene_bgm.wav");
				break;
			default:
				break;
			}
		}
	}
}

int Direct3DMain::soundIndexForSoundId(int soundId)
{
	int soundIndex = soundId - 1;

	return soundIndex;
}

void Direct3DMain::initSoundEngine()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);
	m_retryAudio = false;

	loadSound(L"collect_carrot.wav");
	loadSound(L"collect_golden_carrot.wav");
	loadSound(L"death.wav");
	loadSound(L"footstep_left_grass.wav");
	loadSound(L"footstep_right_grass.wav");
	loadSound(L"footstep_left_cave.wav");
	loadSound(L"footstep_right_cave.wav");
	loadSound(L"jump_spring.wav");
	loadSound(L"landing_grass.wav");
	loadSound(L"landing_cave.wav");
	loadSound(L"snake_death.wav");
	loadSound(L"trigger_transform.wav");
	loadSound(L"cancel_transform.wav");
	loadSound(L"complete_transform.wav");
	loadSound(L"jump_spring_heavy.wav");
	loadSound(L"jon_rabbit_jump.wav");
	loadSound(L"jon_vampire_jump.wav");
	loadSound(L"jon_rabbit_double_jump.wav");
	loadSound(L"jon_rabbit_double_jump.wav");
	loadSound(L"vampire_glide_loop.wav");
	loadSound(L"mushroom_bounce.wav");
	loadSound(L"jon_burrow_rocksfall.wav");
	loadSound(L"sparrow_fly_loop.wav");
	loadSound(L"sparrow_die.wav");
	loadSound(L"toad_die.wav");
	loadSound(L"toad_eat.wav");
	loadSound(L"saw_grind_loop.wav");
	loadSound(L"fox_bounced_on.wav");
	loadSound(L"fox_strike.wav");
	loadSound(L"fox_death.wav");
	loadSound(L"world_1_bgm_intro.wav");
	loadSound(L"mid_boss_bgm_intro.wav");
	loadSound(L"mid_boss_owl_swoop.wav");
	loadSound(L"mid_boss_owl_tree_smash.wav");
	loadSound(L"mid_boss_owl_death.wav");
	loadSound(L"screen_transition.wav");
	loadSound(L"screen_transition_2.wav");
	loadSound(L"level_complete.wav");
	loadSound(L"title_lightning_1.wav");
	loadSound(L"title_lightning_2.wav");
	loadSound(L"ability_unlock.wav");
	loadSound(L"boss_level_clear.wav");
	loadSound(L"level_clear.wav");
	loadSound(L"level_selected.wav");
	loadSound(L"rabbit_drill.wav");
	loadSound(L"snake_jump.wav");
	loadSound(L"vampire_dash.wav");
	loadSound(L"boss_level_unlock.wav");
	loadSound(L"rabbit_stomp.wav");
	loadSound(L"final_boss_bgm_intro.wav");
	loadSound(L"button_click.wav");
	loadSound(L"level_confirmed.wav");
	loadSound(L"bat_poof.wav");
	loadSound(L"chain_snap.wav");
	loadSound(L"end_boss_snake_mouth_open.wav");
	loadSound(L"end_boss_snake_charge_cue.wav");
	loadSound(L"end_boss_snake_charge.wav");
	loadSound(L"end_boss_snake_damaged.wav");
	loadSound(L"end_boss_snake_death.wav");
	loadSound(L"spiked_ball_rolling_loop.wav");
	loadSound(L"absorb_dash_ability.wav");
	loadSound(L"footstep_left_wood.wav");
	loadSound(L"footstep_right_wood.wav");
	loadSound(L"landing_wood.wav");
	loadSound(L"collect_big_carrot.wav");
    loadSound(L"collect_vial.wav");
}

void Direct3DMain::loadSound(const wchar_t* waveFileName)
{
	m_sounds.push_back(std::make_unique<SoundEffect>(m_audEngine.get(), waveFileName));
}

void Direct3DMain::loadMusic(const wchar_t* waveFileName)
{
	if (m_musicLoop.get())
	{
		m_musicLoop->Stop();
		m_musicLoop.reset();
	}

	if (m_music.get())
	{
		m_music.reset();
	}

	m_music = std::make_unique<SoundEffect>(m_audEngine.get(), waveFileName);
	m_musicLoop = m_music->CreateInstance();
}
#pragma endregion

#pragma region Misc
void Direct3DMain::unlockLevel(int requestedAction)
{
	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);
	int score = m_screen->getScore();
	int levelStatsFlag = m_screen->getLevelStatsFlagForUnlockedLevel();
	int numGoldenCarrots = m_screen->getNumGoldenCarrotsAfterUnlockingLevel();

	SaveData::setLevelStatsFlag(world, level, levelStatsFlag);

	SaveData::setNumGoldenCarrots(numGoldenCarrots);
}

void Direct3DMain::markLevelAsCompleted(int requestedAction)
{
	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);
	int score = m_screen->getScore();
	int levelStatsFlag = m_screen->getLevelStatsFlag();
	int numGoldenCarrots = m_screen->getNumGoldenCarrots();
	int jonUnlockedAbilitiesFlag = m_screen->getJonAbilityFlag();

	SaveData::setLevelComplete(world, level, score, levelStatsFlag, jonUnlockedAbilitiesFlag);

	SaveData::setNumGoldenCarrots(numGoldenCarrots);
}

void Direct3DMain::submitScoreOnline(int requestedAction)
{
	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);
	int onlineScore = m_screen->getOnlineScore();

	// TODO, submit score using Xbox Live, on success, save the score that was pushed online

	SaveData::setScorePushedOnline(world, level, onlineScore);
}

void Direct3DMain::setCutsceneViewedFlag(int requestedAction)
{
	while (requestedAction >= 1000)
	{
		requestedAction -= 1000;
	}

	int cutsceneViewedFlag = requestedAction;

	SaveData::setViewedCutscenesFlag(cutsceneViewedFlag);
}

void Direct3DMain::sendSaveData()
{
	int numGoldenCarrots = SaveData::getNumGoldenCarrots();
	int jonUnlockedAbilitiesFlag = SaveData::getJonUnlockedAbilitiesFlag();
	int viewedCutscenesFlag = SaveData::getViewedCutscenesFlag();

	std::stringstream ss;
	ss << "{";
	ss << "\"num_golden_carrots\": " << numGoldenCarrots << ", ";
	ss << "\"jon_unlocked_abilities_flag\": " << jonUnlockedAbilitiesFlag << ", ";
	ss << "\"viewed_cutscenes_flag\": " << viewedCutscenesFlag << ", ";

    int numWorlds = 1;
	for (int i = 1; i <= numWorlds; i++)
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
		if (i < numWorlds)
		{
			ss << ",";
		}
	}
	ss << "}";

	std::string userSaveData = ss.str();

	WorldMap::getInstance()->loadUserSaveData(userSaveData.c_str());
}

int Direct3DMain::calcWorld(int requestedAction)
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

int Direct3DMain::calcLevel(int requestedAction)
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
#pragma endregion

#pragma region Ads
void Direct3DMain::displayInterstitialAdIfAvailable()
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	// Empty
#else
	m_iRequestedAction = 1;
#endif
}
#pragma endregion

#pragma region Level Editor
void Direct3DMain::saveLevel(int requestedAction)
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	const char *level_json = MainScreenLevelEditor::getInstance()->save();
	if (level_json)
	{
		std::stringstream ss;

		ss << "..\\nosfuratu-levels\\";

		int world = calcWorld(requestedAction);
		int level = calcLevel(requestedAction);

		if (world > 0 && level > 0)
		{
			ss << "nosfuratu_c" << world << "_l" << level << ".json";
		}
		else
		{
			ss << "nosfuratu.json";
		}

		std::string ret = ss.str();

		const char *jsonFilePath = ret.c_str();

		FILE *file;
		errno_t err;
		if ((err = fopen_s(&file, jsonFilePath, "w+")) != 0)
		{
            MainScreenLevelEditor::getInstance()->setMessage("Could not find json file...");
		}
		else
		{
			int sum = fprintf(file, "%s", level_json);
			fclose(file);
		}

        MainScreenLevelEditor::getInstance()->setMessage("Level Saved Successfully!");
	}
	else
	{
        MainScreenLevelEditor::getInstance()->setMessage("Error Saving Level...");
	}
#endif
}

void Direct3DMain::loadLevel(int requestedAction)
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	std::stringstream ss;

	ss << "..\\nosfuratu-levels\\";

	int world = calcWorld(requestedAction);
	int level = calcLevel(requestedAction);

	if (world > 0 && level > 0)
	{
		ss << "nosfuratu_c" << world << "_l" << level << ".json";
	}
	else
	{
		ss << "nosfuratu.json";
	}

	std::string ret = ss.str();

	const char *jsonFilePath = ret.c_str();

	FILE *file;
	errno_t err;
	if ((err = fopen_s(&file, jsonFilePath, "r")) != 0)
	{
        MainScreenLevelEditor::getInstance()->setMessage("Could not find json file...");
	}
	else
	{
		// seek to end of file
		fseek(file, 0, SEEK_END);

		// get current file position which is end from seek
		size_t size = ftell(file);

		std::string jsonContent;

		// allocate string space and set length
		jsonContent.resize(size);

		// go back to beginning of file for read
		rewind(file);

		// read 1*size bytes from sfile into ss
		fread(&jsonContent[0], 1, size, file);

		// close the file
		fclose(file);

		MainScreenLevelEditor::getInstance()->load(jsonContent.c_str(), m_screen);
	}
#endif
}
#pragma endregion

// Exit helper
void ExitGame()
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
	PostQuitMessage(0);
#else
	Windows::ApplicationModel::Core::CoreApplication::Exit();
#endif
}
