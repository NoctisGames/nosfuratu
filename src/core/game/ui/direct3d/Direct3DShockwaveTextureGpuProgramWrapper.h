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

#include "pch.h"

class Direct3DProgram;

class Direct3DShockwaveTextureGpuProgramWrapper : public ShockwaveTextureGpuProgramWrapper
{
public:
	Direct3DShockwaveTextureGpuProgramWrapper();
    
    virtual ~Direct3DShockwaveTextureGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
    Direct3DProgram* m_program;
    
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerXConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_centerYConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_timeElapsedConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_isTransformingConstantBuffer;

	void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DShockwaveTextureGpuProgramWrapper__) */
