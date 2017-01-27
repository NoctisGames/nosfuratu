//
//  Direct3DSinWaveTextureGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 10/18/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DSinWaveTextureGpuProgramWrapper__
#define __noctisgames__Direct3DSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
	Direct3DSinWaveTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_offsetConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	void createConstantBuffer();
};

#endif /* defined(__noctisgames__Direct3DSinWaveTextureGpuProgramWrapper__) */
