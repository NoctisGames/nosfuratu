//
//  Direct3DTextureGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DTextureGpuProgramWrapper__
#define __gowengamedev__Direct3DTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DTextureGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

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

#endif /* defined(__gowengamedev__Direct3DTextureGpuProgramWrapper__) */
