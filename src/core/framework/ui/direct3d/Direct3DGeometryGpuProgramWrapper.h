//
//  Direct3DGeometryGpuProgramWrapper.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 8/30/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Direct3DGeometryGpuProgramWrapper__
#define __gowengamedev__Direct3DGeometryGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DGeometryGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DGeometryGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

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

#endif /* defined(__gowengamedev__Direct3DGeometryGpuProgramWrapper__) */
