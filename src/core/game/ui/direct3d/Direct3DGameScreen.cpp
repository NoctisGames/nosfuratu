#include "pch.h"
#include "Direct3DGameScreen.h"
#include "Direct3DRenderer.h"
#include "Direct3DManager.h"
#include "DirectXHelper.h"

using namespace NosFURatu;

using namespace DirectX;
using namespace Windows::Foundation;

Direct3DGameScreen::Direct3DGameScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isMobile, bool useCompressedTextureSet) : GameScreen(), m_deviceResources(deviceResources)
{
	D3DManager->init(m_deviceResources, MAX_BATCH_SIZE, NUM_FRAMEBUFFERS, isMobile);

	Assets::getInstance()->setUsingCompressedTextureSet(useCompressedTextureSet);

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer(m_deviceResources));

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Direct3DGameScreen::CreateDeviceDependentResources()
{
	D3DManager->createDeviceDependentResources();

	m_stateMachine->getCurrentState()->enter(this);

	onResume();
}

// Initializes view parameters when the window size changes.
void Direct3DGameScreen::CreateWindowSizeDependentResources()
{
	D3DManager->createWindowSizeDependentResources();
}

void Direct3DGameScreen::ReleaseDeviceDependentResources()
{
	onPause();

	m_renderer->cleanUp();

	D3DManager->releaseDeviceDependentResources();
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	Size logicalSize = m_deviceResources->GetLogicalSize();
	m_touchPoint->set(touchEvent.getX() / logicalSize.Width * CAM_WIDTH, CAM_HEIGHT - (touchEvent.getY() / logicalSize.Height * CAM_HEIGHT));
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	if (m_stateMachine->getPreviousState())
	{
		Size logicalSize = m_deviceResources->GetLogicalSize();
		onTouch(Touch_Type::UP, logicalSize.Width / 20, logicalSize.Height / 20);
		return true;
	}

	return false;
}