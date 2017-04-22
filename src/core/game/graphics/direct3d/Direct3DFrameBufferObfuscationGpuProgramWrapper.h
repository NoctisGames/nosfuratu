//
//  Direct3DFrameBufferObfuscationGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 9/17/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DFrameBufferObfuscationGpuProgramWrapper__
#define __nosfuratu__Direct3DFrameBufferObfuscationGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class Direct3DProgram;

class Direct3DFrameBufferObfuscationGpuProgramWrapper : public GpuProgramWrapper
{
public:
    Direct3DFrameBufferObfuscationGpuProgramWrapper();
    
    virtual ~Direct3DFrameBufferObfuscationGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
	Direct3DProgram* m_program;
};

#endif /* defined(__nosfuratu__Direct3DFrameBufferObfuscationGpuProgramWrapper__) */
