//
//  Direct3DFrameBufferObfuscationGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DFrameBufferObfuscationGpuProgramWrapper__
#define __noctisgames__Direct3DFrameBufferObfuscationGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DFrameBufferObfuscationGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DFrameBufferObfuscationGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

#endif /* defined(__noctisgames__Direct3DFrameBufferObfuscationGpuProgramWrapper__) */
