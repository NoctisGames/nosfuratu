//
//  Direct3DShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/29/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DShockwaveTextureGpuProgramWrapper__
#define __noctisgames__Direct3DShockwaveTextureGpuProgramWrapper__

#include "ShockwaveTextureGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DShockwaveTextureGpuProgramWrapper : public ShockwaveTextureGpuProgramWrapper
{
public:
	Direct3DShockwaveTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerXConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerYConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_timeElapsedConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_isTransformingConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

	void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DShockwaveTextureGpuProgramWrapper__) */
