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

extern void ExitGame();

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Direct3DMain::Direct3DMain() : m_screen(nullptr), m_isPointerPressed(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
	m_deviceResources->RegisterDeviceNotify(this);
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

	Direct3DManager::setDeviceResources(m_deviceResources.get());

	m_screen = new MainScreen();
	m_screen->onResume();

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
	if (kb.Right)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT);
	}
	if (kb.Up)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP);
	}
	if (kb.Left)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT);
	}
	if (kb.Down)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN);
	}
	if (kb.W)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W);
	}
	if (kb.A)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A);
	}
	if (kb.S)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S);
	}
	if (kb.D)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D);
	}
	if (kb.Enter)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER);
	}
	if (kb.Space)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE);
	}
	if (kb.Back
		|| kb.Delete)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK);
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

	context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
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
#if defined(_DEBUG)
	OutputDebugStringW(L"OnActivated");
#endif

	m_gamePad->Resume();
	m_buttons.Reset();

    // TODO: Game is becoming active window.
	m_screen->onResume();
}

void Direct3DMain::OnDeactivated()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"OnDeactivated");
#endif

	m_gamePad->Suspend();
	m_buttons.Reset();

    // TODO: Game is becoming background window.
	m_screen->onPause();
}

void Direct3DMain::OnSuspending()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"OnSuspending");
#endif

	m_gamePad->Suspend();

	// TODO: Game is being power-suspended (or minimized).
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
#if defined(_DEBUG)
	OutputDebugStringW(L"OnResuming");
#endif

	m_gamePad->Resume();

	// TODO: Game is being power-resumed (or returning from minimize).
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
#if defined(_DEBUG)
	OutputDebugStringW(L"OnWindowSizeChanged");
#endif

	m_fDPI = dpi;

	if (!m_deviceResources->WindowSizeChanged(width, height, rotation))
	{
		return;
	}

	CreateWindowSizeDependentResources();
}

void Direct3DMain::ValidateDevice()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"ValidateDevice");
#endif

	m_deviceResources->ValidateDevice();
}
#endif

// Properties
void Direct3DMain::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Direct3DMain::CreateDeviceDependentResources()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"CreateDeviceDependentResources");
#endif

	m_screen->createDeviceDependentResources();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Direct3DMain::CreateWindowSizeDependentResources()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"CreateWindowSizeDependentResources");
#endif

	RECT outputSize = m_deviceResources->GetOutputSize();
	LONG width = outputSize.right - outputSize.left;
	LONG height = outputSize.bottom - outputSize.top;

	m_screen->createWindowSizeDependentResources(width, height, width, height);
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
#if defined(_DEBUG)
	OutputDebugStringW(L"OnDeviceLost");
#endif

	m_screen->releaseDeviceDependentResources();
}

void Direct3DMain::OnDeviceRestored()
{
#if defined(_DEBUG)
	OutputDebugStringW(L"OnDeviceRestored");
#endif

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
		playSound(soundId);
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
			case MUSIC_LOAD_WORLD_1_LOOP:
				loadMusic(L"music_demo.wav");
				break;
			case MUSIC_PLAY:
				if (m_musicLoop)
				{
					m_musicLoop->Play();
				}
				break;
			case MUSIC_PLAY_LOOP:
				if (m_musicLoop)
				{
					m_musicLoop->Play(true);
				}
				break;
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
			//case MUSIC_STOP:
			//	if (m_musicLoop)
			//	{
			//		m_musicLoop->Stop();
			//	}
				break;
			}
		}
	}
}

void Direct3DMain::playSound(int soundId, bool isLooping)
{
	// TODO
}

void Direct3DMain::playSound(int soundId)
{
	//m_sounds.at(soundId - 1)->Play();
}

void Direct3DMain::stopSound(int soundId)
{
	// TODO
}

void Direct3DMain::stopAllSounds()
{
	// TODO
}

void Direct3DMain::stopAllLoopingSounds()
{
	// TODO
}

void Direct3DMain::initSoundEngine()
{
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);
	m_retryAudio = false;

	//loadSound(L"sound_demo.wav");
}

void Direct3DMain::loadSound(const wchar_t* waveFileName)
{
	m_sounds.push_back(std::make_unique<SoundEffect>(m_audEngine.get(), waveFileName));
}

void Direct3DMain::loadMusic(const wchar_t* waveFileName)
{
	m_music.reset();
	m_music = std::make_unique<SoundEffect>(m_audEngine.get(), waveFileName);

	m_musicLoop.reset();
	m_musicLoop = m_music->CreateInstance();
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
