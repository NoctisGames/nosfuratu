//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"

#include "ScreenInputManager.h"
#include "KeyboardInputManager.h"
#include "Direct3DManager.h"
#include "MainAssets.h"
#include "MainScreen.h"
#include "MainScreenTitle.h"

using namespace NosFURatu;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
using namespace Windows::Phone::UI::Input;
#endif
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace Windows::System::Profile;

DirectXPage::DirectXPage():
	m_windowVisible(true),
	m_isPointerPressed(false),
	m_coreInput(nullptr)
{
	InitializeComponent();

	ApplicationView^ view = ApplicationView::GetForCurrentView();

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	view->SetDesiredBoundsMode(ApplicationViewBoundsMode::UseCoreWindow);
#endif

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::onKeyDown);
    window->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::onKeyUp);

	window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged += ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged += ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	HardwareButtons::BackPressed += ref new EventHandler<BackPressedEventArgs ^>(this, &NosFURatu::DirectXPage::OnBackPressed);
#endif

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

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
    
	bool isMobile = false;
#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP || WINAPI_FAMILY == WINAPI_FAMILY_PC_APP
	isMobile = false;
#elif defined (WINAPI_FAMILY_SYSTEM)
	AnalyticsVersionInfo^ api = AnalyticsInfo::VersionInfo;
	isMobile = api->DeviceFamily->Equals("Windows.Mobile");
#else
	isMobile = true;
#endif

    MAIN_ASSETS->setUsingDesktopTextureSet(!isMobile);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this] (IAsyncAction ^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
			);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<NosFURatuMain>(new NosFURatuMain(m_deviceResources));
	m_main->StartRenderLoop();
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

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

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
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

		m_main->StartRenderLoop();
	}
	else
	{
		m_main->StopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
    
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
    
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DOWN, e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
    
	m_isPointerPressed = true;
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	if (m_isPointerPressed)
	{
        SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_DRAGGED, e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
	}
}

void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
    SCREEN_INPUT_MANAGER->onTouch(ScreenEventType_UP, e->CurrentPoint->Position.X, e->CurrentPoint->Position.Y);
    
	m_isPointerPressed = false;
}

void DirectXPage::onKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
    if (e->VirtualKey == Windows::System::VirtualKey::W)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::S)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Left)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Up)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Right)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Down)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN);
    }
	else if (e->VirtualKey == Windows::System::VirtualKey::Space)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::Enter)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::A)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::D)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::P)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_P);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::M)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_M);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::V)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_V);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::Back
		|| e->VirtualKey == Windows::System::VirtualKey::Delete)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK);
	}
}

void DirectXPage::onKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ e)
{
    if (e->VirtualKey == Windows::System::VirtualKey::W)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_W, true);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::S)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_S, true);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Left)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_LEFT, true);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Up)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_UP, true);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Right)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_RIGHT, true);
    }
    else if (e->VirtualKey == Windows::System::VirtualKey::Down)
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ARROW_KEY_DOWN, true);
    }
	else if (e->VirtualKey == Windows::System::VirtualKey::Space)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_SPACE, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::Enter)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_ENTER, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::A)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_A, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::D)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_D, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::P)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_P, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::M)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_M, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::V)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_V, true);
	}
	else if (e->VirtualKey == Windows::System::VirtualKey::Back
		|| e->VirtualKey == Windows::System::VirtualKey::Delete)
	{
		KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
	}
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void DirectXPage::OnBackPressed(Platform::Object^ sender, BackPressedEventArgs^ args)
{
    if (m_main->getMainScreen()->m_stateMachine.getCurrentState() == Title::getInstance())
    {
        args->Handled = false;
    }
    else
    {
        KEYBOARD_INPUT_MANAGER->onInput(KeyboardEventType_BACK, true);
        
        args->Handled = true;
    }
}
#endif
