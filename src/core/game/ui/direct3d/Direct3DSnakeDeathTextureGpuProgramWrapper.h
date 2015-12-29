//
//  Direct3DSnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/29/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DSnakeDeathTextureGpuProgramWrapper__
#define __gowengamedev__Direct3DSnakeDeathTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DSnakeDeathTextureGpuProgramWrapper : public SnakeDeathTextureGpuProgramWrapper
{
public:
	Direct3DSnakeDeathTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_colorAdditiveConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	void createConstantBuffer();
};

#endif /* defined(__gowengamedev__Direct3DSnakeDeathTextureGpuProgramWrapper__) */
