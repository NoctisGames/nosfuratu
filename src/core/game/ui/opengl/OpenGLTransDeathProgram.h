//
//  OpenGLTransDeathProgram.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__OpenGLTransDeathProgram__
#define __nosfuratu__OpenGLTransDeathProgram__

#include "OpenGLProgram.h"

class OpenGLTransDeathProgram : public OpenGLProgram
{
public:
    GLint u_texture_unit_location;
    GLint u_texture_unit_gray_map_location;
    GLint u_time_elapsed_unit_location;
    GLint a_position_location;
    
    OpenGLTransDeathProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
    
    virtual void bind();
    
    virtual void unbind();
};

#endif /* defined(__nosfuratu__OpenGLTransDeathProgram__) */
