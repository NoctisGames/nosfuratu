//
//  Direct3DSinWaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 10/18/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DSinWaveTextureGpuProgramWrapper__
#define __nosfuratu__Direct3DSinWaveTextureGpuProgramWrapper__

#include "SinWaveTextureGpuProgramWrapper.h"

#include "pch.h"

class Direct3DProgram;

class Direct3DSinWaveTextureGpuProgramWrapper : public SinWaveTextureGpuProgramWrapper
{
public:
	Direct3DSinWaveTextureGpuProgramWrapper();
    
    virtual ~Direct3DSinWaveTextureGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
    Direct3DProgram* m_program;
    
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_offsetConstantBuffer;

	void createConstantBuffer();
};

#endif /* defined(__nosfuratu__Direct3DSinWaveTextureGpuProgramWrapper__) */
