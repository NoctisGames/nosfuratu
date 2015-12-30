#pragma once

#include "GameScreen.h"
#include "MediaEnginePlayer.h"
#include "GameSound.h"
#include "Direct3DProgram.h"
#include "Direct3DTextureGpuProgramWrapper.h"
#include "Direct3DGeometryGpuProgramWrapper.h"
#include "Direct3DFrameBufferToScreenGpuProgramWrapper.h"
#include "DeviceResources.h"
#include "StepTimer.h"

#include <vector>
#include <memory>

namespace NosFURatu
{
	class Direct3DGameScreen : public GameScreen
	{
	public:
		Direct3DGameScreen(const std::shared_ptr<DX::DeviceResources>& deviceResources, int maxBatchSize);
		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

		virtual void onResume();
		virtual void onPause();
		virtual void touchToWorld(TouchEvent &touchEvent);
		bool handleOnBackPressed();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::unique_ptr<MediaEnginePlayer> m_mediaPlayer;
		std::unique_ptr<GameSound> m_collectCarrotSound;
		std::unique_ptr<GameSound> m_collectGoldenCarrotSound;
		std::unique_ptr<GameSound> m_deathSound;
		std::unique_ptr<GameSound> m_footstepLeftGrassSound;
		std::unique_ptr<GameSound> m_footstepRightGrassSound;
		std::unique_ptr<GameSound> m_footstepLeftCaveSound;
		std::unique_ptr<GameSound> m_footstepRightCaveSound;
		std::unique_ptr<GameSound> m_jumpSpringSound;
		std::unique_ptr<GameSound> m_landingGrassSound;
		std::unique_ptr<GameSound> m_landingCaveSound;
        std::unique_ptr<GameSound> m_breakLogSound;
        std::unique_ptr<GameSound> m_destroyRockSound;
        std::unique_ptr<GameSound> m_snakeDeathSound;

		void handleSound();
		void handleMusic();
	};
}

