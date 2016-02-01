//
//  Direct3DTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DTransScreenGpuProgramWrapper__
#define __gowengamedev__Direct3DTransScreenGpuProgramWrapper__

#include "TransitionGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DTransScreenGpuProgramWrapper : public TransitionGpuProgramWrapper
{
public:
	Direct3DTransScreenGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	void createConstantBuffers();
};

#endif /* defined(__gowengamedev__Direct3DTransScreenGpuProgramWrapper__) */
