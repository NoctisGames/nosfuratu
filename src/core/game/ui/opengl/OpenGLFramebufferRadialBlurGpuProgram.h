//
//  OpenGLFramebufferRadialBlurGpuProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/2/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLFramebufferRadialBlurGpuProgram__
#define __nosfuratu__OpenGLFramebufferRadialBlurGpuProgram__

#include "OpenGLProgram.h"

class OpenGLFramebufferRadialBlurGpuProgram : public OpenGLProgram
{
public:
    GLint u_texture_unit_location;
    GLint u_direction_location;
    GLint a_position_location;
    
    OpenGLFramebufferRadialBlurGpuProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
    
    virtual void bind();
    
    virtual void unbind();
};

#endif /* defined(__nosfuratu__OpenGLFramebufferRadialBlurGpuProgram__) */
