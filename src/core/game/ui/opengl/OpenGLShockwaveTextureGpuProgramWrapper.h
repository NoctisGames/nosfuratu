//
//  OpenGLShockwaveTextureGpuProgramWrapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 12/30/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLShockwaveTextureGpuProgramWrapper__
#define __nosfuratu__OpenGLShockwaveTextureGpuProgramWrapper__

#include "ShockwaveTextureGpuProgramWrapper.h"

class OpenGLShockwaveTextureProgram;

class OpenGLShockwaveTextureGpuProgramWrapper : public ShockwaveTextureGpuProgramWrapper
{
public:
    OpenGLShockwaveTextureGpuProgramWrapper();
    
    virtual ~OpenGLShockwaveTextureGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
private:
    OpenGLShockwaveTextureProgram* m_program;
};

#endif /* defined(__nosfuratu__OpenGLShockwaveTextureGpuProgramWrapper__) */
