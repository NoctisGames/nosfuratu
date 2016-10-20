//
//  Direct3DPointTransScreenGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 7/31/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DPointTransScreenGpuProgramWrapper__
#define __noctisgames__Direct3DPointTransScreenGpuProgramWrapper__

#include "PointTransitionGpuProgramWrapper.h"
#include "DeviceResources.h"

class Direct3DPointTransScreenGpuProgramWrapper : public PointTransitionGpuProgramWrapper
{
public:
	Direct3DPointTransScreenGpuProgramWrapper(const std::shared_ptr<DX::DeviceResources>& deviceResources);

	virtual void bind();

	virtual void unbind();

	virtual void cleanUp();

private:
	int m_iNumShadersLoaded;

	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerXConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerYConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_progressConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DPointTransScreenGpuProgramWrapper__) */
