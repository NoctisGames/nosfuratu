//
// pch.h
// Header for standard system include files.
//

#pragma once

#if !defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
	#include <WinSDKVer.h>
	#define _WIN32_WINNT 0x0601
	#include <SDKDDKVer.h>

	// Use the C++ standard templated min/max
	#define NOMINMAX

	// DirectX apps don't need GDI
	#define NODRAWTEXT
	#define NOGDI
	#define NOBITMAP

	// Include <mcx.h> if you need this
	#define NOMCX

	// Include <winsvc.h> if you need this
	#define NOSERVICE

	// WinHelp is deprecated
	#define NOHELP

	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>

	#include <wrl/client.h>

	#include <d3d11_1.h>
	#include <DirectXMath.h>
	#include <DirectXColors.h>

	#include <algorithm>
	#include <exception>
	#include <memory>
	#include <stdexcept>

	#include <stdio.h>
#elif defined (WINAPI_FAMILY_SYSTEM)
	// Use the C++ standard templated min/max
	#define NOMINMAX

	#include <wrl.h>

	#include <d3d11_3.h>
	#include <dxgi1_4.h>
	#include <DirectXMath.h>
	#include <DirectXColors.h>

	#include <algorithm>
	#include <exception>
	#include <memory>
	#include <stdexcept>

	#include <stdio.h>
	#include <pix.h>

	#ifdef _DEBUG
		#include <dxgidebug.h>
	#endif
#else
	#include <wrl.h>
	#include <wrl/client.h>
	#include <dxgi1_4.h>
	#include <d3d11_3.h>
	#include <d2d1_3.h>
	#include <d2d1effects_2.h>
	#include <dwrite_3.h>
	#include <wincodec.h>
	#include <DirectXColors.h>
	#include <DirectXMath.h>
	#include <memory>
	#include <agile.h>
	#include <concrt.h>
#endif
