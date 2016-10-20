//
//  Direct3DBackgroundTextureGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 2/3/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DBackgroundTextureGpuProgramWrapper__
#define __noctisgames__Direct3DBackgroundTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DBackgroundTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DBackgroundTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
};

#endif /* defined(__noctisgames__Direct3DBackgroundTextureGpuProgramWrapper__) */
