#include "pch.h"
#include "Direct3DGameScreen.h"
#include "Direct3DRenderer.h"
#include "Direct3DManager.h"
#include "DirectXHelper.h"

#ifdef GGD_LEVEL_EDITOR
#define IS_LEVEL_EDITOR true
#else
#define IS_LEVEL_EDITOR false
#endif

#define VERTICES_PER_RECTANGLE 4
#define INDICES_PER_RECTANGLE 6

using namespace NosFURatu;

using namespace DirectX;
using namespace Windows::Foundation;

Direct3DGameScreen::Direct3DGameScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize) : GameScreen(IS_LEVEL_EDITOR), m_deviceResources(deviceResources)
{
	D3DManager->init(m_deviceResources, maxBatchSize);

	// Load Sound Effects
	m_collectCarrotSound = std::unique_ptr<GameSound>(new GameSound("assets\\collect_carrot.wav"));
	m_collectGoldenCarrotSound = std::unique_ptr<GameSound>(new GameSound("assets\\collect_golden_carrot.wav"));
	m_deathSound = std::unique_ptr<GameSound>(new GameSound("assets\\death.wav"));

	m_renderer = std::unique_ptr<Direct3DRenderer>(new Direct3DRenderer(m_deviceResources));

	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void Direct3DGameScreen::CreateDeviceDependentResources()
{
	D3DManager->createDeviceDependentResources();

	m_stateMachine->getCurrentState()->enter(this);

	// Load Background Music
	m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
	m_mediaPlayer->Initialize(m_deviceResources->GetD3DDevice(), DXGI_FORMAT_B8G8R8A8_UNORM);

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

	m_mediaPlayer->Shutdown();

	m_renderer->cleanUp();

	D3DManager->releaseDeviceDependentResources();
}

void Direct3DGameScreen::Update(DX::StepTimer const& timer)
{
	switch (getRequestedAction())
	{
	case REQUESTED_ACTION_UPDATE:
		update(timer.GetElapsedSeconds());
		break;
	case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
		clearRequestedAction();
		break;
	case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
		clearRequestedAction();
		break;
	default:
		break;
	}
}

void Direct3DGameScreen::Render()
{
	// Loading is asynchronous, so make sure we are loaded before rendering
	if (m_renderer->isLoaded())
	{
		render();
		handleSound();
		handleMusic();
	}
}

void Direct3DGameScreen::onResume()
{
	GameScreen::onResume();

	GameSound::getSoundPlayerInstance()->Resume();
}

void Direct3DGameScreen::onPause()
{
	GameScreen::onPause();

	GameSound::getSoundPlayerInstance()->Suspend();
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	Size outputSize = m_deviceResources->GetOutputSize();
	m_touchPoint->set(touchEvent.getX() / outputSize.Width * CAM_WIDTH, CAM_HEIGHT - (touchEvent.getY() / outputSize.Height * CAM_HEIGHT));
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	if (m_stateMachine->isInState(*GamePlay::getInstance()))
	{
		Size outputSize = m_deviceResources->GetOutputSize();
		onTouch(Touch_Type::UP, outputSize.Width / 20, outputSize.Height / 20);
		return true;
	}

	return false;
}

void Direct3DGameScreen::handleSound()
{
	short soundId;
	while ((soundId = Assets::getInstance()->getFirstSoundId()) > 0)
	{
		Assets::getInstance()->eraseFirstSoundId();

		switch (soundId)
		{
		case SOUND_COLLECT_CARROT:
			m_collectCarrotSound->play();
			break;
		case SOUND_COLLECT_GOLDEN_CARROT:
			m_collectGoldenCarrotSound->play();
			break;
		case SOUND_DEATH:
			m_deathSound->play();
			break;
		default:
			continue;
		}
	}
}

void Direct3DGameScreen::handleMusic()
{
	short musicId = Assets::getInstance()->getMusicId();
	Assets::getInstance()->setMusicId(0);

	switch (musicId)
	{
	case MUSIC_STOP:
		m_mediaPlayer->Pause();
		break;
	case MUSIC_PLAY_DEMO:
		m_mediaPlayer->SetSource("assets\\bgm.wav");
		m_mediaPlayer->Play();
		break;
	default:
		break;
	}
}