//
//  Direct3DFadeScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/6/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DFadeScreenGpuProgramWrapper__
#define __noctisgames__Direct3DFadeScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DFadeScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
	Direct3DFadeScreenGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;
    bool m_isWindowsMobile;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_isWindowsMobileConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DFadeScreenGpuProgramWrapper__) */
