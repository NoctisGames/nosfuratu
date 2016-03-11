#include "pch.h"
#include "NosFURatuMain.h"
#include "DirectXHelper.h"
#include "GameScreenLevelEditor.h"

using namespace NosFURatu;
using namespace Windows::Foundation;
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
	m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(m_deviceResources, isMobile, isMobile));

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
	m_sounds.push_back("destroy_rock.wav");
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
		case REQUESTED_ACTION_UPDATE:
			m_gameScreen->update(m_timer.GetElapsedSeconds());
			break;
		case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
			saveLevel(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
		case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
			loadLevel(m_gameScreen->getRequestedAction());
			m_gameScreen->clearRequestedAction();
			break;
		default:
			break;
		}
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

	create_task(KnownFolders::PicturesLibrary->GetFolderAsync(L"nosfuratu-levels")).then([this](StorageFolder^ folder)
	{
		if (folder == nullptr)
		{
			displayToast(L"Move the nosfuratu-levels repo to your Pictures folder and try again!");
		}
		else
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
		}
	});
}

void NosFURatuMain::loadLevel(int requestedAction)
{
	m_iRequestedAction = requestedAction;

	create_task(KnownFolders::PicturesLibrary->GetFolderAsync(L"nosfuratu-levels")).then([this](StorageFolder^ folder)
	{
		if (folder == nullptr)
		{
			displayToast(L"Move the nosfuratu-levels repo to your Pictures folder and try again!");
		}
		else
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
		}
	});
}

Platform::String^ NosFURatuMain::getLevelName(int requestedAction)
{
	int world = 0;
	int level = 0;
	while (requestedAction >= 1000)
	{
		requestedAction -= 1000;
	}

	while (requestedAction >= 100)
	{
		requestedAction -= 100;
		world++;
	}

	while (requestedAction >= 1)
	{
		requestedAction--;
		level++;
	}

	if (world > 0 && level > 0)
	{
		return L"nosfuratu_c" + world + L"_l" + level + L".json";
	}
	else
	{
		return L"nosfuratu.json";
	}
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