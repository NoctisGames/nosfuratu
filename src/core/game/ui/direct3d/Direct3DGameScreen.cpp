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

Direct3DGameScreen::Direct3DGameScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize) : GameScreen(IS_LEVEL_EDITOR), m_deviceResources(deviceResources), m_mediaPlayer(nullptr)
{
	D3DManager->init(m_deviceResources, maxBatchSize);

	// Load Sound Effects
	m_collectCarrotSound = std::unique_ptr<GameSound>(new GameSound("collect_carrot.wav"));
	m_collectGoldenCarrotSound = std::unique_ptr<GameSound>(new GameSound("collect_golden_carrot.wav"));
	m_deathSound = std::unique_ptr<GameSound>(new GameSound("death.wav"));
	m_footstepLeftGrassSound = std::unique_ptr<GameSound>(new GameSound("footstep_left_grass.wav"));
	m_footstepRightGrassSound = std::unique_ptr<GameSound>(new GameSound("footstep_right_grass.wav"));
	m_footstepLeftCaveSound = std::unique_ptr<GameSound>(new GameSound("footstep_left_cave.wav"));
	m_footstepRightCaveSound = std::unique_ptr<GameSound>(new GameSound("footstep_right_cave.wav"));
	m_jumpSpringSound = std::unique_ptr<GameSound>(new GameSound("jump_spring.wav"));
	m_landingGrassSound = std::unique_ptr<GameSound>(new GameSound("landing_grass.wav"));
	m_landingCaveSound = std::unique_ptr<GameSound>(new GameSound("landing_cave.wav"));
    m_breakLogSound = std::unique_ptr<GameSound>(new GameSound("break_log.wav"));
    m_destroyRockSound = std::unique_ptr<GameSound>(new GameSound("destroy_rock.wav"));
    m_snakeDeathSound = std::unique_ptr<GameSound>(new GameSound("snake_death.wav"));
    m_triggerTransformSound = std::unique_ptr<GameSound>(new GameSound("trigger_transform.wav"));
    m_cancelTransformSound = std::unique_ptr<GameSound>(new GameSound("cancel_transform.wav"));
    m_completeTransformSound = std::unique_ptr<GameSound>(new GameSound("complete_transform.wav"));

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

	m_mediaPlayer->Shutdown();
	m_mediaPlayer = nullptr;

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

	if (m_mediaPlayer)
	{
		m_mediaPlayer->Pause();
	}
}

void Direct3DGameScreen::touchToWorld(TouchEvent &touchEvent)
{
	Size logicalSize = m_deviceResources->GetLogicalSize();
	m_touchPoint->set(touchEvent.getX() / logicalSize.Width * CAM_WIDTH, CAM_HEIGHT - (touchEvent.getY() / logicalSize.Height * CAM_HEIGHT));
}

bool Direct3DGameScreen::handleOnBackPressed()
{
	if (m_stateMachine->isInState(*GamePlay::getInstance()))
	{
		Size logicalSize = m_deviceResources->GetLogicalSize();
		onTouch(Touch_Type::UP, logicalSize.Width / 20, logicalSize.Height / 20);
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
		case SOUND_FOOTSTEP_LEFT_GRASS:
			m_footstepLeftGrassSound->play();
			break;
		case SOUND_FOOTSTEP_RIGHT_GRASS:
			m_footstepRightGrassSound->play();
			break;
		case SOUND_FOOTSTEP_LEFT_CAVE:
			m_footstepLeftCaveSound->play();
			break;
		case SOUND_FOOTSTEP_RIGHT_CAVE:
			m_footstepRightCaveSound->play();
			break;
		case SOUND_JUMP_SPRING:
			m_jumpSpringSound->play();
			break;
		case SOUND_LANDING_GRASS:
			m_landingGrassSound->play();
			break;
		case SOUND_LANDING_CAVE:
			m_landingCaveSound->play();
			break;
        case SOUND_BREAK_LOG:
            m_breakLogSound->play();
            break;
        case SOUND_DESTROY_ROCK:
            m_destroyRockSound->play();
            break;
        case SOUND_SNAKE_DEATH:
            m_snakeDeathSound->play();
            break;
        case SOUND_TRIGGER_TRANSFORM:
            m_triggerTransformSound->play();
            break;
        case SOUND_CANCEL_TRANSFORM:
            m_cancelTransformSound->play();
            break;
        case SOUND_COMPLETE_TRANSFORM:
            m_completeTransformSound->play();
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
		if (m_mediaPlayer)
		{
			m_mediaPlayer->Shutdown();
			m_mediaPlayer = nullptr;
		}
		break;
    case MUSIC_RESUME:
        m_mediaPlayer->Play();
        break;
	case MUSIC_PLAY_DEMO:
        // Load Background Music
        m_mediaPlayer = std::unique_ptr<MediaEnginePlayer>(new MediaEnginePlayer);
        m_mediaPlayer->Initialize(m_deviceResources->GetD3DDevice(), DXGI_FORMAT_B8G8R8A8_UNORM);
        m_mediaPlayer->SetSource("bgm.wav");
        m_mediaPlayer->Play();
		break;
	default:
		break;
	}
}