#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"
#include "Direct3DGameScreen.h"

// Renders Direct2D and 3D content on the screen.
namespace NosFURatu
{
	class NosFURatuMain : public DX::IDeviceNotify
	{
	public:
		NosFURatuMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~NosFURatuMain();
		void CreateWindowSizeDependentResources();
		void StartRenderLoop();
		void StopRenderLoop();
		void onTouchDown(float screenX, float screenY);
		void onTouchDragged(float screenX, float screenY);
		void onTouchUp(float screenX, float screenY);
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::unique_ptr<Direct3DGameScreen> m_gameScreen;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;
	};
}