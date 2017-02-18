//
//  Direct3DTransDeathGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/28/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__Direct3DTransDeathGpuProgramWrapper__
#define __noctisgames__Direct3DTransDeathGpuProgramWrapper__

#include "TransDeathGpuProgramWrapper.h"

#include "pch.h"

class Direct3DProgram;

class Direct3DTransDeathGpuProgramWrapper : public TransDeathGpuProgramWrapper
{
public:
	Direct3DTransDeathGpuProgramWrapper(bool isTransIn);

	virtual void bind();

	virtual void unbind();

private:
    Direct3DProgram* m_program;
    
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_timeElapsedConstantBuffer;

	void createConstantBuffers();
};

#endif /* defined(__noctisgames__Direct3DTransDeathGpuProgramWrapper__) */
