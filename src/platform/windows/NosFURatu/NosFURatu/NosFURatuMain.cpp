#include "pch.h"
#include "NosFURatuMain.h"
#include "DirectXHelper.h"

using namespace NosFURatu;
using namespace Windows::Foundation;
using namespace Windows::System::Profile;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
NosFURatuMain::NosFURatuMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	AnalyticsVersionInfo^ api = AnalyticsInfo::VersionInfo;

	bool isUsingCompressedTextureSet = false;
	if (api->DeviceFamily->Equals("Windows.Mobile"))
	{
		m_timer.SetFixedTimeStep(true);
		m_timer.SetTargetElapsedSeconds(1.0 / 30.0);
		isUsingCompressedTextureSet = true;
	}

	m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(m_deviceResources, MAX_BATCH_SIZE, isUsingCompressedTextureSet));
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
}

void NosFURatuMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
	m_gameScreen->onPause();
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
}

// Notifies renderers that device resources may now be recreated.
void NosFURatuMain::OnDeviceRestored()
{
	m_gameScreen->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void NosFURatuMain::Update() 
{
	m_timer.Tick([&]()
	{
		m_gameScreen->Update(m_timer);
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

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_gameScreen->Render();

	return true;
}