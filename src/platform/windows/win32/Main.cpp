//
// Main.cpp
//

#include "pch.h"
#include "Direct3DMain.h"
#include "Audio.h"
#include <Dbt.h>

using namespace DirectX;

namespace
{
    std::unique_ptr<Direct3DMain> g_main;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Entry point
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (!XMVerifyCPUSupport())
	{
		return 1;
	}

    HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
	{
		return 1;
	}

    g_main = std::make_unique<Direct3DMain>();

    // Register class and create window
    {
        // Register class
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, L"IDI_ICON");
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"NosFURatuWindowClass";
        wcex.hIconSm = LoadIcon(wcex.hInstance, L"IDI_ICON");
		if (!RegisterClassEx(&wcex))
		{
			return 1;
		}

        // Create window
        int w, h;
        g_main->GetDefaultSize(w, h);

        RECT rc;
        rc.top = 0;
        rc.left = 0;
        rc.right = static_cast<LONG>(w); 
        rc.bottom = static_cast<LONG>(h);

        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        HWND hwnd = CreateWindowEx(WS_EX_TOPMOST, L"NosFURatuWindowClass", L"NosFURatu", WS_POPUP,
            CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
            nullptr);

		if (!hwnd)
		{
			return 1;
		}

        ShowWindow(hwnd, SW_SHOWMAXIMIZED);

        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(g_main.get()) );

        GetClientRect(hwnd, &rc);

        g_main->Initialize(hwnd, rc.right - rc.left, rc.bottom - rc.top);

		HDEVNOTIFY hNewAudio = nullptr;

		// Listen for new audio devices
		DEV_BROADCAST_DEVICEINTERFACE filter = { 0 };
		filter.dbcc_size = sizeof(filter);
		filter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		filter.dbcc_classguid = KSCATEGORY_AUDIO;

		hNewAudio = RegisterDeviceNotification(hwnd, &filter,
			DEVICE_NOTIFY_WINDOW_HANDLE);

		if (hNewAudio)
		{
			UnregisterDeviceNotification(hNewAudio);
		}
    }

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_main->Tick();
		}
	}

	g_main.reset();

	CoUninitialize();

	return (int)msg.wParam;
}

// Windows procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    static bool s_in_sizemove = false;
    static bool s_in_suspend = false;
    static bool s_minimized = false;
    static bool s_fullscreen = true;

    auto direct3DMain = reinterpret_cast<Direct3DMain*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;

    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
        {
            if (!s_minimized)
            {
                s_minimized = true;
				if (!s_in_suspend && direct3DMain)
				{
					direct3DMain->OnSuspending();
				}
                s_in_suspend = true;
            }
        }
        else if (s_minimized)
        {
            s_minimized = false;
			if (s_in_suspend && direct3DMain)
			{
				direct3DMain->OnResuming();
			}
            s_in_suspend = false;
        }
        else if (!s_in_sizemove && direct3DMain)
        {
            direct3DMain->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_ENTERSIZEMOVE:
        s_in_sizemove = true;
        break;

    case WM_EXITSIZEMOVE:
        s_in_sizemove = false;
        if (direct3DMain)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            direct3DMain->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
        }
        break;

    case WM_GETMINMAXINFO:
        {
            auto info = reinterpret_cast<MINMAXINFO*>(lParam);
            info->ptMinTrackSize.x = 320;
            info->ptMinTrackSize.y = 200;
        }
        break;

    case WM_ACTIVATEAPP:
        if (direct3DMain)
        {
            if (wParam)
            {
                direct3DMain->OnActivated();
            }
            else
            {
                direct3DMain->OnDeactivated();
            }
        }

		Keyboard::ProcessMessage(message, wParam, lParam);
		Mouse::ProcessMessage(message, wParam, lParam);
        
		break;

    case WM_POWERBROADCAST:
        switch (wParam)
        {
        case PBT_APMQUERYSUSPEND:
			if (!s_in_suspend && direct3DMain)
			{
				direct3DMain->OnSuspending();
			}
            s_in_suspend = true;
            return TRUE;

        case PBT_APMRESUMESUSPEND:
            if (!s_minimized)
            {
				if (s_in_suspend && direct3DMain)
				{
					direct3DMain->OnResuming();
				}
                s_in_suspend = false;
            }
            return TRUE;
        }
        break;

    case WM_DESTROY:
		PostQuitMessage(0);
        break;

    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
        {
            // Implements the classic ALT+ENTER fullscreen toggle
            if (s_fullscreen)
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

                int width = 800;
                int height = 600;
				if (direct3DMain)
				{
					direct3DMain->GetDefaultSize(width, height);
				}

                ShowWindow(hWnd, SW_SHOWNORMAL);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            else
            {
                SetWindowLongPtr(hWnd, GWL_STYLE, 0);
                SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

                ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            }

            s_fullscreen = !s_fullscreen;
        }

		Keyboard::ProcessMessage(message, wParam, lParam);

        break;

    case WM_MENUCHAR:
        // A menu is active and the user presses a key that does not correspond
        // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
        return MAKELRESULT(0, MNC_CLOSE);

	case WM_INPUT:
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		Mouse::ProcessMessage(message, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		Keyboard::ProcessMessage(message, wParam, lParam);
		break;

	case WM_DEVICECHANGE:
		if (wParam == DBT_DEVICEARRIVAL)
		{
			auto pDev = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
			if (pDev)
			{
				if (pDev->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
				{
					auto pInter = reinterpret_cast<const PDEV_BROADCAST_DEVICEINTERFACE>(pDev);
					if (pInter->dbcc_classguid == KSCATEGORY_AUDIO)
					{
						if (direct3DMain)
						{
							direct3DMain->OnNewAudioDevice();
						}
					}
				}
			}
		}
		return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
