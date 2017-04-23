//
//  Direct3DBackgroundTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/3/16.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__Direct3DBackgroundTextureGpuProgramWrapper__
#define __nosfuratu__Direct3DBackgroundTextureGpuProgramWrapper__

#include "GpuProgramWrapper.h"

class Direct3DProgram;

class Direct3DBackgroundTextureGpuProgramWrapper : public GpuProgramWrapper
{
public:
	Direct3DBackgroundTextureGpuProgramWrapper();

    virtual ~Direct3DBackgroundTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    Direct3DProgram* m_program;
};

#endif /* defined(__nosfuratu__Direct3DBackgroundTextureGpuProgramWrapper__) */
