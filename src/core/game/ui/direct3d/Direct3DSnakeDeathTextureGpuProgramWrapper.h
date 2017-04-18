//
//  Direct3DSnakeDeathTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/29/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DSnakeDeathTextureGpuProgramWrapper__
#define __nosfuratu__Direct3DSnakeDeathTextureGpuProgramWrapper__

#include "SnakeDeathTextureGpuProgramWrapper.h"

class Direct3DProgram;

class Direct3DSnakeDeathTextureGpuProgramWrapper : public SnakeDeathTextureGpuProgramWrapper
{
public:
	Direct3DSnakeDeathTextureGpuProgramWrapper();
    
    virtual ~Direct3DSnakeDeathTextureGpuProgramWrapper();

	virtual void bind();

	virtual void unbind();

private:
	Direct3DProgram* m_program;
};

#endif /* defined(__nosfuratu__Direct3DSnakeDeathTextureGpuProgramWrapper__) */
