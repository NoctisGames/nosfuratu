//
//  Direct3DShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/29/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DShockwaveTextureGpuProgramWrapper__
#define __nosfuratu__Direct3DShockwaveTextureGpuProgramWrapper__

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
};

#endif /* defined(__nosfuratu__Direct3DShockwaveTextureGpuProgramWrapper__) */
