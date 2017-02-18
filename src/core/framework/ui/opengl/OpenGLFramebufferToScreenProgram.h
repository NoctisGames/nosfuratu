//
//  OpenGLFramebufferToScreenProgram.h
//  noctisgames-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2017 Noctis Games. All rights reserved.
//

#ifndef __noctisgames__OpenGLFramebufferToScreenProgram__
#define __noctisgames__OpenGLFramebufferToScreenProgram__

extern "C"
{
#include "platform_gl.h"
}

typedef struct
{
    GLuint program;
    
    GLint a_position_location;
    GLint u_texture_unit_location;
    GLint u_direction_location;
} OpenGLFramebufferToScreenProgramStruct;

class OpenGLFramebufferToScreenProgram
{
public:
    static OpenGLFramebufferToScreenProgramStruct build(GLuint program);
    
private:
    OpenGLFramebufferToScreenProgram();
};

#endif /* defined(__noctisgames__OpenGLFramebufferToScreenProgram__) */
