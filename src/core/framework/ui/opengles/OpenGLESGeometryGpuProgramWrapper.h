//
//  OpenGLESGeometryGpuProgramWrapper.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 8/27/15.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLESGeometryGpuProgramWrapper__
#define __noctisgames__OpenGLESGeometryGpuProgramWrapper__

#include "GpuProgramWrapper.h"
#include "ColorProgram.h"

class OpenGLESGeometryGpuProgramWrapper : public GpuProgramWrapper
{
public:
    OpenGLESGeometryGpuProgramWrapper();
    
    virtual void bind();
    
    virtual void unbind();
    
    virtual void cleanUp();
    
private:
    ColorProgramStruct m_program;
};

#endif /* defined(__noctisgames__OpenGLESGeometryGpuProgramWrapper__) */
