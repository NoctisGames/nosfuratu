//
//  Direct3DTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DTransDeathGpuProgramWrapper__
#define __gowengamedev__Direct3DTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
	Direct3DTransDeathGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources, bool isTransIn);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_timeElapsedConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	void createConstantBuffers();
};

#endif /* defined(__gowengamedev__Direct3DTransDeathGpuProgramWrapper__) */
