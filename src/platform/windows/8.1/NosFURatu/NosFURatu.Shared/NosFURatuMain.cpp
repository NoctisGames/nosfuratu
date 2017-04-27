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
NosFURatuMain::NosFURatuMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources), m_iRequestedAction(0)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);
    
	NG_AUDIO_ENGINE->update(1);

    m_screen = new MainScreen();

	m_screen->createDeviceDependentResources();
}

NosFURatuMain::~NosFURatuMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void NosFURatuMain::CreateWindowSizeDependentResources()
{
#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
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
    
	Size renderTargetSize = m_deviceResources->GetRenderTargetSize();
	Size logicalSize = m_deviceResources->GetLogicalSize();

	bool isLargeEnoughForReduction = renderTargetSize.Width > 2048 || renderTargetSize.Height > 2048;
	UINT renderWidth = D3DManager->isWindowsMobile() && isLargeEnoughForReduction ? static_cast<UINT>(renderTargetSize.Width * 0.5f + 0.5f) : renderTargetSize.Width;
	UINT renderHeight = D3DManager->isWindowsMobile() && isLargeEnoughForReduction ? static_cast<UINT>(renderTargetSize.Height * 0.5f + 0.5f) : renderTargetSize.Height;

	m_screen->createWindowSizeDependentResources(renderWidth, renderHeight, logicalSize.Width, logicalSize.Height);
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
}

MainScreen* NosFURatuMain::getMainScreen()
{
	return m_screen;
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
#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
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
				m_iRequestedAction = 1;
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
