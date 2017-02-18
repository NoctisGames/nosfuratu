//
//  OpenGLShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLShockwaveTextureGpuProgramWrapper__
#define __noctisgames__OpenGLShockwaveTextureGpuProgramWrapper__

#include "ShockwaveTextureGpuProgramWrapper.h"
#include "OpenGLShockwaveTextureProgram.h"

class OpenGLShockwaveTextureGpuProgramWrapper : public ShockwaveTextureGpuProgramWrapper
{
public:
    OpenGLShockwaveTextureGpuProgramWrapper();
    
    virtual ~OpenGLShockwaveTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLShockwaveTextureProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLShockwaveTextureGpuProgramWrapper__) */
