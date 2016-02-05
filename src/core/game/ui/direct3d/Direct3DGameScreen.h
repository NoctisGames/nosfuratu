#pragma once

#include "GameScreen.h"
#include "MediaEnginePlayer.h"
#include "GameSound.h"
#include "Direct3DProgram.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DGeometryGpuProgramWrapper.h"
#include "Direct3DFramebufferToScreenGpuProgramWrapper.h"
#include "DeviceResources.h"
#include "StepTimer.h"

#include <vector>
#include <memory>

namespace NosFURatu
{
	class Direct3DGameScreen : public GameScreen
	{
	public:
		Direct3DGameScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isMobile, bool useCompressedTextureSet);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		virtual void touchToWorld(TouchEvent &touchEvent);

		bool handleOnBackPressed();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
	};
}

