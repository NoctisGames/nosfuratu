//
//  NosFURatuMain.cpp
//  nosfuratu
//
//  Created by Stephen Gowen on 4/20/17.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#include "pch.h"

#include "NosFURatuMain.h"

#include "MainScreen.h"

#include "Direct3DManager.h"
#include "MainAssets.h"
#include "GameConstants.h"
#include "macros.h"
#include "NGAudioEngine.h"

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
    
    m_screen = new MainScreen();
    
    NG_AUDIO_ENGINE->update(1);
}

NosFURatuMain::~NosFURatuMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void NosFURatuMain::CreateWindowSizeDependentResources()
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
    static const XMFLOAT4X4 Rotation0(
                                      1.0f, 0.0f, 0.0f, 0.0f,
                                      0.0f, 1.0f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 0.0f,
                                      0.0f, 0.0f, 0.0f, 1.0f
                                      );
    Direct3DManager::init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext(), m_deviceResources->GetBackBufferRenderTargetView(), Rotation0);
#else
    Direct3DManager::init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext(), m_deviceResources->GetBackBufferRenderTargetView(), m_deviceResources->GetOrientationTransform3D());
#endif
    
	m_screen->CreateWindowSizeDependentResources();
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

	m_screen->onResume();
}

void NosFURatuMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
	m_screen->onPause();

	NG_AUDIO_ENGINE->update(-1);
}

// Notifies renderers that device resources need to be released.
void NosFURatuMain::OnDeviceLost()
{
	m_screen->releaseDeviceDependentResources();

    NG_AUDIO_ENGINE->update(-1);
}

// Notifies renderers that device resources may now be recreated.
void NosFURatuMain::OnDeviceRestored()
{
#if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
    static const XMFLOAT4X4 Rotation0(
                                      1.0f, 0.0f, 0.0f, 0.0f,
                                      0.0f, 1.0f, 0.0f, 0.0f,
                                      0.0f, 0.0f, 1.0f, 0.0f,
                                      0.0f, 0.0f, 0.0f, 1.0f
                                      );
    Direct3DManager::init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext(), m_deviceResources->GetBackBufferRenderTargetView(), Rotation0);
#else
    Direct3DManager::init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext(), m_deviceResources->GetBackBufferRenderTargetView(), m_deviceResources->GetOrientationTransform3D());
#endif
    
	m_screen->createDeviceDependentResources();
    
	CreateWindowSizeDependentResources();
    
    NG_AUDIO_ENGINE->update(1);
}

void NosFURatuMain::Update()
{
	m_timer.Tick([&]()
	{
        int requestedAction = m_screen->getRequestedAction();
        
        switch (requestedAction)
        {
            case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
                m_directXPage->DisplayInterstitialAdIfAvailable();
                m_screen->clearRequestedAction();
                break;
            case REQUESTED_ACTION_UPDATE:
                break;
            default:
                m_screen->clearRequestedAction();
                break;
        }

		m_screen->update(m_timer.GetElapsedSeconds());
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

	m_screen->render();

	return true;
}

void NosFURatuMain::handleMusic()
{
	short rawMusicId;
	while ((rawMusicId = m_screen->getCurrentMusicId()) > 0)
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
